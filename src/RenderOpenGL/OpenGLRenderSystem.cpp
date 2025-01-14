#include "OpenGLRenderSystem.hpp"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <GLFW/WindowGLFW.hpp>

#include <iostream>
#include <memory>
#include <vector>

#include "EngineTime.hpp"

#include "PositionVertexLayoutDescriptor.hpp"
#include "BasicShapeFactory.hpp"
#include "Materials/BasicShapeMaterialImpl.hpp"

#include  "BaseRenderPass.hpp"

namespace Snowglobe::RenderOpenGL
{
    OpenGLRenderSystem* OpenGLRenderSystem::_instance = nullptr;

    OpenGLRenderSystem::OpenGLRenderSystem() :
        _shaderCompiler(std::make_unique<ShaderCompiler>())
    {
        _requiersUpdate = true;
        _instance = this;

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    OpenGLRenderSystem::~OpenGLRenderSystem()
    {
        glfwTerminate();
    }

    void OpenGLRenderSystem::Update()
    {
        SnowCore::EngineTime::GetInstance()->RenderTick();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        _camera.Update();

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        for(auto& _mesh : _meshes)
        {
            RenderPassSignature signature(_mesh);
            auto renderPass = _renderPasses.find(signature);
            if(renderPass == _renderPasses.end())
            {
                std::cout << "Render pass not found" << std::endl;
                continue;
            }

            renderPass->second->Execute(_mesh);
        }
    }

    void OpenGLRenderSystem::InitializeWindow(const Render::WindowParams& params)
    {
        _mainWindow = std::make_unique<Render::GLFW::WindowGLFW>(params);

        //_camera = Render::Camera(Render::CameraMode::Orthographic, 45.0f, 1.0f, 0.1f, 100.0f);
        _camera.SetFov(45.0f);
        _camera.SetWidth(params.width);
        _camera.SetHeight(params.height);
        _camera.SetOrthographicSize(5);
        _camera.SetPosition(glm::vec3(0.0f, 0.0f, -3.0f));
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
    }
    
    void OpenGLRenderSystem::InitializeRenderScene()
    {
        _vertexLayoutDescriptors.insert({typeid(Render::PositionVertex), PositionVertexLayoutDescriptor::GetInstance()});
        RegisterMaterialManager<Materials::BasicShapeMaterialImpl, Render::BasicShapeMaterial>();

        auto renderPass = std::make_unique<BaseRenderPass>();
        _renderPasses.insert({renderPass->GetSignature(), std::move(renderPass)});
    }

    Render::MeshProxy* OpenGLRenderSystem::CreateMeshProxy(const Render::VertexBufferPtr& vertexBuffer,
        const std::string& debugName)
    {
        _meshes.emplace_back(_nextMeshId, (const VertexBufferPtrOpenGL&) vertexBuffer, debugName);
        _nextMeshId = { _nextMeshId.id + 1 };
        return &_meshes.back();
    }

    Render::MeshProxy *OpenGLRenderSystem::CreateMeshProxy(const Render::VertexBufferPtr &vertexBuffer,
                                                           const Render::IndexBufferPtr &indexBuffer,
                                                           const std::string& debugName)
    {
        _meshes.emplace_back(_nextMeshId, (const VertexBufferPtrOpenGL&) vertexBuffer, (const IndexBufferPtrOpenGL&) indexBuffer, debugName);
        _nextMeshId = { _nextMeshId.id + 1 };
        return &_meshes.back();
    }

    Render::IndexBufferPtr* OpenGLRenderSystem::AllocateIndexBufferPtr(const std::vector<uint32_t>& indices,
        const std::string& debugName)
    {
        _indexBuffers.emplace_back(indices, debugName);
        return &_indexBuffers.back();
    }

    Render::VertexBufferPtr* OpenGLRenderSystem::AllocateVertexBufferPtrImpl(std::type_index vertexType,
                                                                             size_t vertexCount, const void* bufferPtr, const std::string& debugName)
    {
        auto vertexDesc = _vertexLayoutDescriptors.find(vertexType);
        if(vertexDesc == _vertexLayoutDescriptors.end())
        {
            std::cout << "Vertex type not supported" << std::endl;
            return nullptr;
        }
        
        uint32_t stride = vertexDesc->second->GetStride();
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