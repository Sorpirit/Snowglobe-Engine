#include "Sprite2DTest.hpp"

#include "SpriteRenderComponent.hpp"
#include "TransformComponent.hpp"

namespace Snowglobe {

void Sprite2DTest::Init()
{
    RuntimeTest::Init();

    auto manager = _engine->GetEntityManager();
    auto entity = manager->CreateEntity();
    entity->AddComponent<Core::TransformComponent>();
    auto sprite = entity->AddOrGetComponent<Render::SpriteRenderComponent>();
    auto assets = DI->Resolve<Core::AssetManager>();
    sprite->SpriteAsset.Set(assets->Get<Render::TextureAssetData>("grass.png"));
    sprite->Color = glm::vec3(1.0f, 1.0f, 1.0f);
    sprite->TileSize = sprite->Size();
}

void Sprite2DTest::Run() {}

} // Snowglobe