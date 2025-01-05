#include <iostream>

#include "SnowEngine.hpp"
#include "RenderSystem.hpp"
#include "OpenGLRenderSystem.hpp"
#include "VulkanRenderingSystem.hpp"

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

    void SnowEngine::Setup(const SnowCore::EngineProfile& profile)
    {
        switch (profile.preferredRenderEngine)
        {
        case SnowCore::EngineRenderEngine::OpenGL:
            TryAddSystem<Render::RenderSystem>(new RenderOpenGL::OpenGLRenderSystem());
            break;
        case SnowCore::EngineRenderEngine::Vulkan:
            TryAddSystem<Render::RenderSystem>(new RenderVulkan::VulkanRenderingSystem());
            break;
        case SnowCore::EngineRenderEngine::DirectX:
            std::cout << "DirectX is not implemented yet" << std::endl;
            break;
        default:
            std::cout << "Unknown render engine" << std::endl;
            break;
        }
    }

    void SnowEngine::Update()
    {
        for(auto& system : _systems)
        {
            if(system.second->RequiersUpdate())
                system.second->Update();
        }
    }

    
} // namespace Snowglobe::SnowEngine