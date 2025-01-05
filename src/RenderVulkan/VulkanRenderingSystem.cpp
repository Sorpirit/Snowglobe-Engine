#include "VulkanRenderingSystem.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <GLFW/WindowGLFW.hpp>

#include <iostream>

namespace Snowglobe::RenderVulkan
{
    VulkanRenderingSystem::VulkanRenderingSystem()
    {
        _requiersUpdate = true;

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }
    
    VulkanRenderingSystem::~VulkanRenderingSystem()
    {
        glfwTerminate();
    }

    void VulkanRenderingSystem::Update()
    {
        _mainWindow->Present();
        _mainWindow->PollEvents();
    }

    void VulkanRenderingSystem::InitializeWindow(const Render::WindowParams& params)
    {
        _mainWindow = std::make_unique<Render::GLFW::WindowGLFW>(params);
    }
}