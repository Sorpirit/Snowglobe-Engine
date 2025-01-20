#include <iostream>
#include <utility>

#include "SnowEngine.hpp"
#include "RenderSystem.hpp"
#include "OpenGLRenderSystem.hpp"
//#include "VulkanRenderingSystem.hpp"
#include "Imgui/ImguiSystem.hpp"
#include "PhysicsEngine2DSystem.hpp"
#include "RenderEngineSyncSystem.hpp"
#include "EngineTime.hpp"

namespace Snowglobe::SnowEngine
{
    SnowEngine::~SnowEngine()
    {
        _systems.clear();
    }

    void SnowEngine::Setup(
        const SnowCore::EngineProfile& profile,
        const Render::WindowParams& windowParams,
        const std::shared_ptr<SnowCore::ECS::EntityManagerBase>& entityManager)
    {
        _entityManager = std::move(entityManager);

        bool success = true;
        switch (profile.preferredRenderEngine)
        {
        case SnowCore::EngineRenderEngine::OpenGL:
            _systems[typeid(Render::RenderSystem)] = std::make_shared<RenderOpenGL::OpenGLRenderSystem>(*_entityManager.get());
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
        
        _systems[typeid(Render::UISystem)] = std::make_shared<RenderOpenGL::Imgui::ImguiSystem>(renderSystem->GetMainWindow(), *_entityManager);
        
        Render::UISystem* uiSystem = nullptr;
        if(!QuerySystem<Render::UISystem>(uiSystem))
        {
            std::cout << "Failed to get Imgui system" << std::endl;
            return;
        }
        renderSystem->SetUISystem(uiSystem);

        _systems[typeid(PhysicsEngine2DSystem)] = std::make_shared<PhysicsEngine2DSystem>(*_entityManager);
        _systems[typeid(RenderEngineSyncSystem)] = std::make_shared<RenderEngineSyncSystem>(*_entityManager);
        
        renderSystem->InitializeRenderScene();
    }

    void SnowEngine::Run()
    {
        SnowCore::EngineTime::GetInstance()->EngineTick();
        for(const auto& [type, system] : _systems)
        {
            if(system->IsActive())
                system->UpdateEarly();
        }

        for (auto& updateFunction : _updateCallbacks)
        {
            updateFunction();
        }
        
        for(const auto& [type, system] : _systems)
        {
            if(system->IsActive())
                system->Update();
        }
        
        for(const auto& [type, system] : _systems)
        {
            if(system->IsActive())
                system->UpdateLate();
        }

        _entityManager->Update();
    }

} // namespace Snowglobe::SnowEngine