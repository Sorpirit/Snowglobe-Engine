#include "InputActionsTest.hpp"

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

    auto& input = _engine->GetInputActionSystem();
    input.RegisterInputConfig<TestInputConfig>();
    auto inputConfig = input.GetInputMapping<TestInputConfig>();
    inputConfig->WalkLeft.RegisterCallback([=](bool isStarted) {
        if (isStarted)
            animation->SetAnimationIndex(static_cast<int>(AnimationStates::Walking));
        else
            animation->SetAnimationIndex(static_cast<int>(AnimationStates::Idle));
    });
    inputConfig->WalkRight.RegisterCallback([=](bool isStarted) {
        if (isStarted)
            animation->SetAnimationIndex(static_cast<int>(AnimationStates::Runing));
        else
            animation->SetAnimationIndex(static_cast<int>(AnimationStates::Idle));
    });

    input.SetMapping(Core::Key::D, inputConfig->WalkLeft);
    input.SetMapping(Core::Key::A, inputConfig->WalkLeft);
    input.SetMapping(Core::Key::Left, inputConfig->WalkLeft);
    input.SetMapping(Core::Key::Right, inputConfig->WalkLeft);
}

void InputActionsTest::Run()
{
}

} // Snowglobe