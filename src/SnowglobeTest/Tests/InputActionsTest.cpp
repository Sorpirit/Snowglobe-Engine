#include "InputActionsTest.hpp"

#include "Collider2DComponent.hpp"
#include "Physics2DComponent.hpp"
#include "SpriteAnimationComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "TransformComponent.hpp"

namespace Snowglobe {

enum class AnimationStates
{
    Idle = 0,
    Walking,
    Runing,
    Jumping,
    Falling,
};


void InputActionsTest::Init()
{
    RuntimeTest::Init();

    auto manager = _engine->GetEntityManager();
    auto assets = DI->Resolve<Core::AssetManager>();

    Player = manager->CreateEntity();
    Player->AddComponent<Core::TransformComponent>();
    Player->AddComponent<Engine::Collider2DComponent>(Engine::CollisionShapeType::AABB, false);
    auto physics = Player->AddOrGetComponent<Engine::Physics2DComponent>();
    physics->Bounciness = .2f;

    auto sprite = Player->AddOrGetComponent<Render::SpriteRenderComponent>();
    sprite->SpriteAsset.Set(*assets->Get<Render::SpriteAssetData>("runner.sprite"));
    sprite->Color = glm::vec3(1.0f, 1.0f, 1.0f);

    auto animation = Player->AddOrGetComponent<Render::SpriteAnimationComponent>();
    animation->AnimationClips[static_cast<int>(AnimationStates::Idle)] = {{0, 0}, {4, 0}, true, 1 / 8.0};
    animation->AnimationClips[static_cast<int>(AnimationStates::Walking)] = {{0, 1}, {8, 1}, true, 1 / 8.0};
    animation->AnimationClips[static_cast<int>(AnimationStates::Runing)] = {{0, 2}, {5, 2}, true, 1 / 8.0};
    animation->AnimationClips[static_cast<int>(AnimationStates::Jumping)] = {{0, 3}, {0, 3}, false, 1 / 8.0};
    animation->AnimationClips[static_cast<int>(AnimationStates::Falling)] = {{1, 3}, {1, 3}, false, 1 / 8.0};
    animation->SetAnimationIndex(static_cast<int>(AnimationStates::Idle));

    auto& input = _engine->GetInputActionSystem();
    input.RegisterInputConfig<TestInputConfig>();
    auto inputConfig = input.GetInputMapping<TestInputConfig>();
    inputConfig->WalkLeft.RegisterCallback([=](bool isStarted) {
        if (isStarted)
        {
            sprite->flipX = true;
            animation->SetAnimationIndex(static_cast<int>(AnimationStates::Runing));
            physics->Velocity.x = -1;
        }
        else
        {
            animation->SetAnimationIndex(static_cast<int>(AnimationStates::Idle));
            physics->Velocity.x = 0;
        }
    });
    inputConfig->WalkRight.RegisterCallback([=](bool isStarted) {
        if (isStarted)
        {
            sprite->flipX = false;
            animation->SetAnimationIndex(static_cast<int>(AnimationStates::Runing));
            physics->Velocity.x = 1;
        }
        else
        {
            animation->SetAnimationIndex(static_cast<int>(AnimationStates::Idle));
            physics->Velocity.x = 0;
        }
    });
    inputConfig->Jump.RegisterCallback([=](bool isStarted) {
        if (isStarted)
        {
            animation->SetAnimationIndex(static_cast<int>(AnimationStates::Jumping));
            physics->Velocity.y = 5;
        }
        else
        {
            animation->SetAnimationIndex(static_cast<int>(AnimationStates::Falling));
            physics->Velocity.y = 0;
        }
    });

    input.SetMapping(Core::Key::A, inputConfig->WalkLeft);
    input.SetMapping(Core::Key::D, inputConfig->WalkRight);
    input.SetMapping(Core::Key::Left, inputConfig->WalkLeft);
    input.SetMapping(Core::Key::Right, inputConfig->WalkRight);
    input.SetMapping(Core::Key::W, inputConfig->Jump);
    input.SetMapping(Core::Key::Space, inputConfig->Jump);

    auto entity = manager->CreateEntity();
    auto transform = entity->AddOrGetComponent<Core::TransformComponent>();
    transform->Position = glm::vec3(0, -1, 0);
    transform->Scale = glm::vec3(10, 1, 1);
    auto sprite2 = entity->AddOrGetComponent<Render::SpriteRenderComponent>();
    sprite2->SpriteAsset.Set(*assets->Get<Render::SpriteAssetData>("grass.sprite"));
    sprite2->Color = glm::vec3(1.0f, 1.0f, 1.0f);
    entity->AddComponent<Engine::Collider2DComponent>(Engine::CollisionShapeType::AABB, false);
}

void InputActionsTest::Run()
{
}

} // Snowglobe