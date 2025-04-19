#pragma once
#include "../RuntimeTest.hpp"
#include "Editor/SpectatorCameraSystem.hpp"
#include <nlohmann/json.hpp>

namespace Snowglobe
{

class SceneSerializationTest : public RuntimeTest
{
  public:
    SceneSerializationTest() : RuntimeTest("SceneSerializationTest"), _spectator(&_renderSystem->GetCamera(), &_window->GetInput()) {}
    void Init() override;
    void Run() override;

  private:
    Engine::SpectatorCameraSystem _spectator;

    bool _saved = false;
    nlohmann::json _sceneJson;
    Core::ECS::Lifetime _sceneLifetime;
};

} // namespace Snowglobe
