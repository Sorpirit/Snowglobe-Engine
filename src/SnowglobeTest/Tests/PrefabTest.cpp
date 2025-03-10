#include "PrefabTest.hpp"

#include "Assets/PrefabAssetData.hpp"
#include "SpriteRenderComponent.hpp"
#include "TransformComponent.hpp"

namespace Snowglobe {

void PrefabTest::Init()
{
    RuntimeTest::Init();

    auto manager = _engine->GetEntityManager();
    auto assets = DI->Resolve<Core::AssetManager>();

    auto prefab = assets->Get<Core::PrefabAssetData>("Block.prefab");
    auto instance = manager->CreateEntity(*prefab->GetData().DetachedEntity);
}

void PrefabTest::Run() {}

} // Snowglobe