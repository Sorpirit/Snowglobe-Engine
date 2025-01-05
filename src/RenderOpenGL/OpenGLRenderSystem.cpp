#include "OpenGLRenderSystem.hpp"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <GLFW/WindowGLFW.hpp>

#include <iostream>

namespace Snowglobe::RenderOpenGL
{
    OpenGLRenderSystem::OpenGLRenderSystem()
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
}