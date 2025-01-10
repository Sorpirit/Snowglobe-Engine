#include <iostream>

#include "SnowEngine.hpp"
#include "RenderSystem.hpp"
#include "OpenGLRenderSystem.hpp"
#include "VulkanRenderingSystem.hpp"
#include "Imgui/ImguiSystem.hpp"

namespace Snowglobe::SnowEngine
{
    SnowEngine::~SnowEngine()
    {
        for(auto& system : _systems)
        {
            delete system.second;
        }
        _systems.clear();
    }

    void SnowEngine::Setup(const SnowCore::EngineProfile& profile, const Snowglobe::Render::WindowParams& windowParams)
    {
        bool success = false;
        switch (profile.preferredRenderEngine)
        {
        case SnowCore::EngineRenderEngine::OpenGL:
            success = TryAddSystem<Render::RenderSystem>(new RenderOpenGL::OpenGLRenderSystem());
            break;
        case SnowCore::EngineRenderEngine::Vulkan:
            success = TryAddSystem<Render::RenderSystem>(new RenderVulkan::VulkanRenderingSystem());
            break;
        case SnowCore::EngineRenderEngine::DirectX:
            std::cout << "DirectX is not implemented yet" << std::endl;
            break;
        default:
            std::cout << "Unknown render engine" << std::endl;
            break;
        }

        if(!success)
        {
            std::cout << "Failed to add render system" << std::endl;
        }

        Snowglobe::Render::RenderSystem* renderSystem = nullptr; 
        if(!QuerySystem<Snowglobe::Render::RenderSystem>(renderSystem))
        {
            std::cout << "Failed to get render system" << std::endl;
            return;
        }

        renderSystem->InitializeWindow(windowParams);
        renderSystem->InitializeRenderScene();

        if(!TryAddSystem<RenderOpenGL::Imgui::ImguiSystem>(new RenderOpenGL::Imgui::ImguiSystem(renderSystem->GetMainWindow())))
        {
            std::cout << "Failed to add Imgui system" << std::endl;
        }
    }

    void SnowEngine::Update()
    {
        for(auto& system : _frameSystems)
        {
            system->EarlyUpdate();
        }

        for(auto& system : _systems)
        {
            if(system.second->RequiersUpdate())
                system.second->Update();
        }
    }

    
} // namespace Snowglobe::SnowEngine