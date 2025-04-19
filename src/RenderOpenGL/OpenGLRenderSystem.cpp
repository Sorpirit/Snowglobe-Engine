#include "OpenGLRenderSystem.hpp"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <GLFW/WindowGLFW.hpp>

#include <iostream>
#include <memory>
#include <variant>
#include <vector>

#include "EngineTime.hpp"

#include "CommonVertexLayoutDescriptor.hpp"

#include "BasicShapeFactory.hpp"
#include "ObjectOutlineRenderPass.hpp"

#include "Materials/BasicShapeMaterialImpl.hpp"
#include "Materials/OutlineTextureLitMaterialImpl.hpp"
#include "Materials/TextureLitMaterialImpl.hpp"
#include "Materials/TextureShapeMaterialImpl.hpp"

#include "TextureManager.hpp"
#include "TransformComponent.hpp"

#include "TemplateRenderPass.hpp"

namespace Snowglobe::RenderOpenGL
{
    template <typename MaterialImpl, typename MaterialData>
    void OpenGLRenderSystem::RegisterMaterialManager()
    {
        _materialManages.insert({typeid(MaterialData), Materials::TemplateMaterialManager<MaterialImpl, MaterialData>::GetInstance()});
    }

    template <typename MaterialImpl, typename InstanceVertexLayoutDescriptor>
    void OpenGLRenderSystem::RegisterTemplateRenderPass(const Core::SnowFileHandle& vertexShader, const Core::SnowFileHandle& fragmentShader, bool useLighting)
    {
        auto pass = std::make_unique<TemplateRenderPass<MaterialImpl, InstanceVertexLayoutDescriptor>>(vertexShader, fragmentShader, useLighting);
        _renderPasses.insert({pass->GetSignature(), std::move(pass)});
    }

    OpenGLRenderSystem* OpenGLRenderSystem::_instance = nullptr;

    OpenGLRenderSystem::OpenGLRenderSystem() : _shaderCompiler(std::make_unique<ShaderCompiler>())
    {
        _instance = this;

        _updateOrder = Core::ECS::UpdateOrder::Render;

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    OpenGLRenderSystem::~OpenGLRenderSystem()
    {
        glDeleteFramebuffers(1, &_frameBuffer);
        glfwTerminate();
    }

    void OpenGLRenderSystem::Update()
    {
        Core::EngineTime::GetInstance()->RenderTick();

        glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        _camera.Update();
        _lightParameters.Reset();
        for (auto lightEntity : _entityManager->GetEntitiesWithTag(Tags::Lights()))
        {
            Core::TransformComponent* transform = nullptr;
            if (!lightEntity->QueryComponent(transform))
                continue;

            DirectionalLightComponent* directional;
            if (lightEntity->QueryComponent(directional))
            {
                auto& directionalRef = _lightParameters.GetDirectionalLight();
                directionalRef = directional->Light;
                directionalRef.LightDirection = -transform->GetUp();
            }

            PointLightComponent* pointLight;
            if (lightEntity->QueryComponent(pointLight))
            {
                auto& pointLightRef = _lightParameters.NextPointLight();
                pointLightRef = pointLight->Light;
                pointLightRef.LightPosition = transform->Position;
            }

            SpotLightComponent* spotLight;
            if (lightEntity->QueryComponent(spotLight))
            {
                auto& spotLightRef = _lightParameters.NextSpotLight();
                spotLightRef = spotLight->Light;
                spotLightRef.LightPosition = transform->Position;
                spotLightRef.LightDirection = -transform->GetUp();
            }
        }

        for(auto& mesh : _meshes)
        {
            if (mesh.GetMaterial() == nullptr || mesh.GetVertexBuffer() == nullptr || !mesh.IsVisible())
                continue;
            
            RenderPassSignature signature(mesh);
            auto renderPass = _renderPasses.find(signature);
            if(renderPass == _renderPasses.end())
            {
                std::cout << "Render pass not found" << std::endl;
                continue;
            }

            renderPass->second->Execute(mesh);
        }

        _shape2DSystem.Update();
        _spriteRenderer.Update();
        _skybox.Draw();
        _gizmos.Draw();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        _quadProgram->Bind();
        glBindVertexArray(_quadVAO);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, _colorTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        if (_mainWindow->GetInput().IsKeyPressed(Core::Key::F5))
        {
            _shaderCompiler->RecompileAll();
        }

        _uiSystem->EndRendering();
    }

    void OpenGLRenderSystem::InitializeWindow(const Render::WindowParams& params)
    {
        _mainWindow = std::make_unique<Render::GLFW::WindowGLFW>(params);

        //_camera = Render::Camera(Render::CameraMode::Orthographic, 45.0f, 1.0f, 0.1f, 100.0f);
        _camera.SetFov(45.0f);
        _camera.SetWidth(params.Width);
        _camera.SetHeight(params.Height);
        _camera.SetOrthographicSize(5);
        _camera.SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));
        _camera.SetMode(Render::CameraMode::Orthographic);

        _mainWindow->SetResizeCallback([this](int width, int height)
        {
            glViewport(0, 0, width, height);
            _camera.SetWidth(width);
            _camera.SetHeight(height);
        });
        
        if (!gladLoadGL(glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }

        glGenFramebuffers(1, &_frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

        glGenTextures(1, &_colorTexture);
        glBindTexture(GL_TEXTURE_2D, _colorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, params.Width, params.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        uint32_t depthTexture;
        glGenTextures(1, &depthTexture);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, params.Width, params.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorTexture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        std::array positionsUV {
            glm::vec2(-1, -1), glm::vec2(0, 0),
            glm::vec2(-1, 1), glm::vec2(0, 1),
            glm::vec2(1, 1), glm::vec2(1, 1),
            glm::vec2(-1, -1), glm::vec2(0, 0),
            glm::vec2(1, 1), glm::vec2(1, 1),
            glm::vec2(1, -1), glm::vec2(1, 0),
        };
        uint32_t quadVBO;
        glGenBuffers(1, &quadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float) * positionsUV.size(), positionsUV.data(), GL_STATIC_DRAW);

        glGenVertexArrays(1, &_quadVAO);
        glBindVertexArray(_quadVAO);

        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);


        GraphicsPipelineCreateInfo quadProgramParams;
        quadProgramParams.VertexShader = Core::SnowFileHandle("screenQuad.vert");
        quadProgramParams.FragmentShader = Core::SnowFileHandle("screenQuad.frag");
        _quadProgram = _shaderCompiler->GetOrCratePipeline(quadProgramParams);
    }
    
    void OpenGLRenderSystem::InitializeRenderScene()
    {
        _vertexLayoutDescriptors.insert({typeid(Render::PositionVertex), PositionVertexLayoutDescriptor::GetInstance()});
        _vertexLayoutDescriptors.insert({typeid(Render::PositionUVVertex), PositionUVVertexLayoutDescriptor::GetInstance()});
        _vertexLayoutDescriptors.insert({typeid(Render::PositionNormalUVVertex), PositionNormalUVVertexLayoutDescriptor::GetInstance()});
        _vertexLayoutDescriptors.insert({typeid(Render::PositionNormalTangentUVVertex), PositionNormalTangentUVVertexLayoutDescriptor::GetInstance()});

        _shape2DSystem.Init(_entityManager);
        _spriteRenderer.Init(_entityManager);
        _skybox.Init();
        _gizmos.Init();
        
        RegisterMaterialManager<Materials::BasicShapeMaterialImpl, Render::BasicShapeMaterial>();
        RegisterMaterialManager<Materials::TextureShapeMaterialImpl, Render::MaterialsData::TextureColorMaterialData>();
        RegisterMaterialManager<Materials::TextureLitMaterialImpl, Render::MaterialsData::TextureLitMaterialData>();
        RegisterMaterialManager<Materials::OutlineTextureLitMaterialImpl, Render::MaterialsData::TextureLitOutlineMaterialData>();

        RegisterTemplateRenderPass<Materials::BasicShapeMaterialImpl, PositionVertexLayoutDescriptor>(
            Core::SnowFileHandle("color.vert"), Core::SnowFileHandle("color.frag"));

        RegisterTemplateRenderPass<Materials::TextureShapeMaterialImpl, PositionUVVertexLayoutDescriptor>(
            Core::SnowFileHandle("textureUnlit.vert"), Core::SnowFileHandle("textureUnlit.frag"));

        RegisterTemplateRenderPass<Materials::TextureLitMaterialImpl, PositionNormalUVVertexLayoutDescriptor>(
            Core::SnowFileHandle("textureLit.vert"), Core::SnowFileHandle("textureLit.frag"), true);

        auto litPass = _renderPasses.at({typeid(Materials::TextureLitMaterialImpl), typeid(PositionNormalUVVertexLayoutDescriptor)}).get();
        auto outlinePass = std::make_unique<ObjectOutlineRenderPass>(litPass);
        _renderPasses.insert({outlinePass->GetSignature(), std::move(outlinePass)});

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
    }

    Render::MeshProxy* OpenGLRenderSystem::CreateMeshProxy(const Render::VertexBufferPtr& vertexBuffer,
        const std::string& debugName)
    {
        _meshes.emplace_back(_nextMeshId, (const VertexBufferPtrOpenGL&) vertexBuffer, debugName);
        _nextMeshId = { _nextMeshId.GetId() + 1 };
        return &_meshes.back();
    }

    Render::MeshProxy *OpenGLRenderSystem::CreateMeshProxy(const Render::VertexBufferPtr &vertexBuffer,
                                                           const Render::IndexBufferPtr &indexBuffer,
                                                           const std::string& debugName)
    {
        _meshes.emplace_back(_nextMeshId, (const VertexBufferPtrOpenGL&) vertexBuffer, (const IndexBufferPtrOpenGL&) indexBuffer, debugName);
        _nextMeshId = { _nextMeshId.GetId() + 1 };
        return &_meshes.back();
    }

    Render::IndexBufferPtr* OpenGLRenderSystem::AllocateIndexBufferPtr(const std::vector<uint32_t>& indices,
        const std::string& debugName)
    {
        _indexBuffers.emplace_back(indices, debugName);
        return &_indexBuffers.back();
    }

    Render::Texture2DPtr OpenGLRenderSystem::CreateTexture2D(const Core::FileTexture &texture,
                                                             const Render::Texture2DDescriptor &desc,
                                                             const std::string &debugName)
    {
        return TextureManager::GetInstance().CreateTexture2D(texture, desc, debugName);
    }

    Render::VertexBufferPtr *OpenGLRenderSystem::AllocateVertexBufferPtrImpl(std::type_index vertexType,
                                                                             size_t vertexCount, const void *bufferPtr,
                                                                             const std::string &debugName)
    {
        auto vertexDesc = _vertexLayoutDescriptors.find(vertexType);
        if(vertexDesc == _vertexLayoutDescriptors.end())
        {
            std::cout << "Vertex type not supported" << std::endl;
            return nullptr;
        }
        
        _vertexBuffers.emplace_back(vertexDesc->second, vertexCount, bufferPtr, debugName);
        return &_vertexBuffers.back();
    }

    Render::MaterialBase* OpenGLRenderSystem::CreateMaterialInstanceImpl(std::type_index materialType,const std::string &debugName)
    {
        auto materialManager = _materialManages.find(materialType);
        if(materialManager == _materialManages.end())
        {
            std::cout << "Material type not supported" << std::endl;
            return nullptr;
        }

        return materialManager->second->CreateMaterial(debugName);
    }

    } // namespace Snowglobe::RenderOpenGL