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

        glUseProgram(_shaderProgram);
        _mesh->Bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        _mesh->Unbind();

        _mainWindow->Present();
        _mainWindow->PollEvents();
    }

    void OpenGLRenderSystem::InitializeWindow(const Render::WindowParams& params)
    {
        _mainWindow = std::make_unique<Render::GLFW::WindowGLFW>(params);

        if (!gladLoadGL(glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }
    }
    
    void OpenGLRenderSystem::InitializeRenderScene()
    {
        auto vertecies = std::make_unique<std::vector<PositionColorVertex>>(std::initializer_list<PositionColorVertex>{
            {glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
            {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
            {glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)}
        });

        auto indices = std::make_unique<std::vector<unsigned int>>(std::initializer_list<unsigned int>{
            0, 1, 2
        });
        auto mesh = Mesh<PositionColorVertex>::Create(*PositionColorVertexDescriptor::GetInstance(), std::move(vertecies), std::move(indices));
        _mesh = std::unique_ptr<Mesh<PositionColorVertex>>(mesh);

        auto vertexShader = _shaderCompiler->GetOrCompileShader(SnowCore::SnowFileHandle("color.vert"));
        auto fragmentShader = _shaderCompiler->GetOrCompileShader(SnowCore::SnowFileHandle("color.frag"));
        PipelineSetupParams params = { vertexShader, fragmentShader };
        _shaderProgram = _shaderCompiler->GetOrCratePipeline(params);
    }
    
    } // namespace Snowglobe::RenderOpenGL