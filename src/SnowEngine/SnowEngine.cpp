#include <iostream>

#include "SnowEngine.hpp"
#include "RenderSystem.hpp"
#include "OpenGLRenderSystem.hpp"
//#include "VulkanRenderingSystem.hpp"
#include "Imgui/ImguiSystem.hpp"
#include "PhysicsEngine2DSystem.hpp"
#include "EngineTime.hpp"

namespace Snowglobe::SnowEngine
{
    SnowEngine::~SnowEngine()
    {
        //todo: delete data from systems
        // for(auto& entity : _entities)
        // {
        //     entity.Destroy();
        // }
        // _entities.clear();

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
            //todo disable vulkan until implemented
            //success = TryAddSystem<Render::RenderSystem>(new RenderVulkan::VulkanRenderingSystem());
            std::cout << "Vulkan is not implemented yet" << std::endl;
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

        Render::RenderSystem* renderSystem = nullptr; 
        if(!QuerySystem<Render::RenderSystem>(renderSystem))
        {
            std::cout << "Failed to get render system" << std::endl;
            return;
        }

        renderSystem->InitializeWindow(windowParams);

        if(!TryAddSystem<RenderOpenGL::Imgui::ImguiSystem>(new RenderOpenGL::Imgui::ImguiSystem(renderSystem->GetMainWindow())))
        {
            std::cout << "Failed to add Imgui system" << std::endl;
        }
        
        RenderOpenGL::Imgui::ImguiSystem* imguiSystem = nullptr;
        if(!QuerySystem<RenderOpenGL::Imgui::ImguiSystem>(imguiSystem))
        {
            std::cout << "Failed to get Imgui system" << std::endl;
            return;
        }

        if(!TryAddSystem(new PhysicsEngine2DSystem()))
        {
            std::cout << "Failed to add Imgui system" << std::endl;
        }
        
        renderSystem->SetUISystem(imguiSystem);
        renderSystem->InitializeRenderScene();
    }

    void SnowEngine::StartFrame() const
    {
        for(auto& system : _frameSystems)
        {
            system->EarlyUpdate();
        }
    }

    void SnowEngine::Update() const
    {
        SnowCore::EngineTime::GetInstance()->EngineTick();

        for (auto& entity : _entities)
        {
            entity.Update();
        }
        
        for(auto& system : _systems)
        {
            if(system.second->RequiersUpdate())
                system.second->Update();
        }
    }

    
} // namespace Snowglobe::SnowEngine