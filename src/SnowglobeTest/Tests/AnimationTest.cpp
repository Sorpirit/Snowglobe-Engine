#include "AnimationTest.hpp"

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


void AnimationTest::Init()
{
    RuntimeTest::Init();

    auto manager = _engine->GetEntityManager();
    Player = manager->CreateEntity();
    Player->AddComponent<Core::TransformComponent>();
    auto sprite = Player->AddOrGetComponent<Render::SpriteRenderComponent>();
    auto assets = DI->Resolve<Core::AssetManager>();
    sprite->SpriteAsset.Set(*assets->Get<Render::SpriteAssetData>("runner.sprite"));
    sprite->Color = glm::vec3(1.0f, 1.0f, 1.0f);
    auto animation = Player->AddOrGetComponent<Render::SpriteAnimationComponent>();

    animation->AnimationClips[static_cast<int>(AnimationStates::Idle)] = {{0, 0}, {4, 0}, true, 1 / 8.0};
    animation->AnimationClips[static_cast<int>(AnimationStates::Walking)] = {{0, 1}, {8, 1}, true, 1 / 8.0};
    animation->AnimationClips[static_cast<int>(AnimationStates::Runing)] = {{0, 2}, {5, 2}, true, 1 / 8.0};
    animation->AnimationClips[static_cast<int>(AnimationStates::Jumping)] = {{0, 3}, {0, 3}, false, 1 / 8.0};
    animation->AnimationClips[static_cast<int>(AnimationStates::Falling)] = {{1, 3}, {1, 3}, false, 1 / 8.0};
    animation->SetAnimationIndex(static_cast<int>(AnimationStates::Idle));
}

void AnimationTest::Run()
{
    auto animation = Player->AddOrGetComponent<Render::SpriteAnimationComponent>();
    if (_input->IsKeyHeld(Core::Key::Q))
        animation->SetAnimationIndex(static_cast<int>(AnimationStates::Idle));
    else if (_input->IsKeyHeld(Core::Key::W))
        animation->SetAnimationIndex(static_cast<int>(AnimationStates::Walking));
    else if (_input->IsKeyHeld(Core::Key::E))
        animation->SetAnimationIndex(static_cast<int>(AnimationStates::Runing));
    else if (_input->IsKeyHeld(Core::Key::R))
        animation->SetAnimationIndex(static_cast<int>(AnimationStates::Jumping));
    else if (_input->IsKeyHeld(Core::Key::T))
        animation->SetAnimationIndex(static_cast<int>(AnimationStates::Falling));
}

} // Snowglobe