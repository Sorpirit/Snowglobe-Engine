#pragma once
#include "../../../thirdparty/tracy/import/src/json.hpp"
#include "../../Core/Serialization/SerializationAPI.hpp"
#include "../RuntimeTest.hpp"
#include "SpectatorCamera.hpp"

namespace Snowglobe
{

class SceneSerializationTest : public RuntimeTest
{
  public:
    SceneSerializationTest() : RuntimeTest("SceneSerializationTest"), _spectator(&_renderSystem->GetCamera(), &_window->GetInput()) {}
    void Init() override;
    void Run() override;

  private:
    Engine::SpectatorCamera _spectator;

    bool _saved = false;
    nlohmann::json _sceneJson;
    Core::ECS::Lifetime _sceneLifetime;
};

} // namespace Snowglobe
