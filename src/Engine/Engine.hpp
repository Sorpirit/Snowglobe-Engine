#pragma once

#include <string>

#include "Assets/AssetManager.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/LifetimeSystem.hpp"
#include "EngineProfile.hpp"
#include "SceneManager.hpp"
#include "Window.hpp"

namespace Snowglobe::Engine
{

class Engine
{
  public:
    Engine() = default;
    ~Engine();

    void Setup(const Core::EngineProfile& profile, const Render::WindowParams& windowParams,
               std::shared_ptr<Core::ECS::EntityManagerBase> entityManager);
    void Run();

    std::shared_ptr<Core::ECS::EntityManagerBase> GetEntityManager() const { return _entityManager; }
    std::shared_ptr<Core::ECS::SystemManager> GetSystemManager() const { return _systemManager; }
    Core::ECS::LifetimeSystem& GetLifetimeSystem() { return _lifetimeSystem; }
    SceneManager& GetSceneManager() { return _sceneManager; }

  private:
    std::shared_ptr<Core::ECS::EntityManagerBase> _entityManager = nullptr;
    std::shared_ptr<Core::ECS::SystemManager> _systemManager = nullptr;

    Core::ECS::LifetimeSystem _lifetimeSystem;
    SceneManager _sceneManager;

    std::string _applicationName;
};

} // namespace Snowglobe::Engine
