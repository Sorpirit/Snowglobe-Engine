#include "SceneSerializationTest.hpp"

namespace Snowglobe
{

void SceneSerializationTest::Init()
{
    RuntimeTest::Init();

    _engine->GetSceneManager().LoadScene("SceneSerializationTest.scene");
}

void SceneSerializationTest::Run()
{
    // if (_input->IsKeyPressed(Core::Key::F1))
    // {
    //     // Save
    //     SaveScene();
    // }
    //
    // if (_input->IsKeyPressed(Core::Key::F2))
    // {
    //     // Load
    //     LoadScene();
    // }
    //
    // if (_input->IsKeyPressed(Core::Key::F3))
    // {
    //     // DestoryScene
    //     _engine->GetLifetimeSystem().Expire(_sceneLifetime);
    //     _sceneLifetime = {0};
    // }
}

} // namespace Snowglobe