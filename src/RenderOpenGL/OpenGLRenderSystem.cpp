#include "OpenGLRenderSystem.hpp"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <GLFW/WindowGLFW.hpp>

#include <iostream>
#include <memory>
#include <vector>

namespace Snowglobe::RenderOpenGL
{
    OpenGLRenderSystem::OpenGLRenderSystem() : 
        _shaderCompiler(std::make_unique<ShaderCompiler>())
    {
        _requiersUpdate = true;

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
    
    OpenGLRenderSystem::~OpenGLRenderSystem()
    {
        glfwTerminate();
    }

    void OpenGLRenderSystem::Update()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        _camera.Update();

        glUseProgram(_shaderProgram);
        _mesh->Bind();
        _entity.Bind(_shaderProgram);
        _material.Bind(_shaderProgram);
        _sceneParameters.Bind(_camera, _shaderProgram);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        _mesh->Unbind();
    }

    void OpenGLRenderSystem::InitializeWindow(const Render::WindowParams& params)
    {
        _mainWindow = std::make_unique<Render::GLFW::WindowGLFW>(params);

        //_camera = Render::Camera(Render::CameraMode::Orthographic, 45.0f, 1.0f, 0.1f, 100.0f);
        _camera.SetFov(45.0f);
        _camera.SetAspectRatio(static_cast<float>(params.width) / static_cast<float>(params.height));
        _camera.SetOrthographicSize(5);
        _camera.SetPosition(glm::vec3(0.0f, 0.0f, -3.0f));
        _camera.SetMode(Render::CameraMode::Orthographic);

        if (!gladLoadGL(glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }
    }
    
    void OpenGLRenderSystem::InitializeRenderScene()
    {
        // auto vertecies = std::make_unique<std::vector<PositionColorVertex>>(std::initializer_list<PositionColorVertex>{
        //     {glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
        //     {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
        //     {glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)}
        // });

        auto vertecies = std::make_unique<std::vector<PositionVertex>>(std::initializer_list<PositionVertex>{
            {glm::vec3(0.5f, -0.5f, 0.0f)},
            {glm::vec3(-0.5f, -0.5f, 0.0f)},
            {glm::vec3(0.0f, 0.5f, 0.0f)}
        });

        auto indices = std::make_unique<std::vector<unsigned int>>(std::initializer_list<unsigned int>{
            0, 1, 2
        });
        auto mesh = Mesh<PositionVertex>::Create(*PositionVertexDescriptor::GetInstance(), std::move(vertecies), std::move(indices));
        _mesh = std::unique_ptr<Mesh<PositionVertex>>(mesh);

        _entity.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        _entity.SetScale(glm::vec3(1.0f, 1.0f, 1.0f) * 3.0f);

        _material.SetColor(glm::vec3(1.0f, 0.0f, 0.0f));

        auto vertexShader = _shaderCompiler->GetOrCompileShader(SnowCore::SnowFileHandle("color.vert"));
        auto fragmentShader = _shaderCompiler->GetOrCompileShader(SnowCore::SnowFileHandle("color.frag"));
        PipelineSetupParams params = { vertexShader, fragmentShader };
        _shaderProgram = _shaderCompiler->GetOrCratePipeline(params);
    }
    
    } // namespace Snowglobe::RenderOpenGL