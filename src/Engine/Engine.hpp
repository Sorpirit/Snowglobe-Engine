#pragma once

#include <string>

#include "ECS/EntityManager.hpp"
#include "ECS/LifetimeSystem.hpp"
#include "EngineProfile.hpp"
#include "InputActionSystem.hpp"
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
    InputActionSystem& GetInputActionSystem() { return _inputActionSystem; }

  private:
    std::shared_ptr<Core::ECS::EntityManagerBase> _entityManager = nullptr;
    std::shared_ptr<Core::ECS::SystemManager> _systemManager = nullptr;

    Core::ECS::LifetimeSystem _lifetimeSystem;
    InputActionSystem _inputActionSystem;
    SceneManager _sceneManager;

    std::string _applicationName;
};

} // namespace Snowglobe::Engine
