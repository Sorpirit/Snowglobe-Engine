#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <typeinfo>
#include <functional>

#include "ECS/EntityManager.hpp"
#include "ECS/ISystem.hpp"
#include "ECS/LifetimeSystem.hpp"
#include "EngineProfile.hpp"
#include "LifetimeSystem.hpp"
#include "Window.hpp"

namespace Snowglobe::Engine
{

class Engine
{
public:
    static Engine& GetInstance()
    {
        static Engine instance;
        return instance;
    }
    
    ~Engine();

    void Setup(
        const Core::EngineProfile& profile,
        const Render::WindowParams& windowParams,
        std::shared_ptr<Core::ECS::EntityManagerBase> entityManager);
    void Run();

    std::shared_ptr<Core::ECS::EntityManagerBase> GetEntityManager() const { return _entityManager; }
    std::shared_ptr<Core::ECS::SystemManager> GetSystemManager() const { return _systemManager; }
    Core::ECS::LifetimeSystem& GetLifetimeSystem() { return _lifetimeSystem; }

private:
    Engine() = default;

    std::shared_ptr<Core::ECS::EntityManagerBase> _entityManager = nullptr;
    std::shared_ptr<Core::ECS::SystemManager> _systemManager = nullptr;

    Core::ECS::LifetimeSystem _lifetimeSystem;

    std::string _applicationName;
};
    
}

