#include "Physics2DTest.hpp"

#include "Collider2DComponent.hpp"
#include "Physics2DComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "TransformComponent.hpp"

namespace Snowglobe
{

void Physics2DTest::Init()
{
    RuntimeTest::Init();

    auto manager = _engine->GetEntityManager();
    auto entity = manager->CreateEntity();
    auto transform = entity->AddOrGetComponent<Core::TransformComponent>();
    transform->Position = glm::vec3(0, -1, 0);
    transform->Scale = glm::vec3(10, 1, 1);
    auto sprite = entity->AddOrGetComponent<Render::SpriteRenderComponent>();
    auto assets = DI->Resolve<Core::AssetManager>();
    sprite->SpriteAsset.Set(*assets->Get<Render::SpriteAssetData>("grass.sprite"));
    sprite->Color = glm::vec3(1.0f, 1.0f, 1.0f);
    entity->AddComponent<Engine::Collider2DComponent>(Engine::CollisionShapeType::AABB, false);

    auto entity2 = manager->CreateEntity();
    auto transform2 = entity2->AddOrGetComponent<Core::TransformComponent>();
    transform2->Position = glm::vec3(0, 2, 0);
    auto sprite2 = entity2->AddOrGetComponent<Render::SpriteRenderComponent>();
    sprite2->SpriteAsset.Set(*assets->Get<Render::SpriteAssetData>("grass.sprite"));
    sprite2->Color = glm::vec3(1.0f, 1.0f, 1.0f);
    entity2->AddComponent<Engine::Collider2DComponent>(Engine::CollisionShapeType::AABB, false);
    auto physics2 = entity2->AddOrGetComponent<Engine::Physics2DComponent>();
    physics2->Bounciness = .2f;
    _playerBlock = entity2;
}

void Physics2DTest::Run()
{
    float horizontal = 0.0;
    if (_input->IsKeyHeld(Core::Key::A))
    {
        horizontal = -1.0f;
    }
    else if (_input->IsKeyHeld(Core::Key::D))
    {
        horizontal = 1.0;
    }

    float vertical = 0.0;
    if (_input->IsKeyPressed(Core::Key::Space))
    {
        vertical = 5.0f;
    }

    auto physics = _playerBlock->AddOrGetComponent<Engine::Physics2DComponent>();
    physics->Velocity.x = horizontal;
    if (vertical > 0)
        physics->Velocity.y += vertical;

}

} // namespace Snowglobe