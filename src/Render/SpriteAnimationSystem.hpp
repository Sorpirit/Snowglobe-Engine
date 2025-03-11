#pragma once
#include "ECS/ISystem.hpp"
#include "EngineTime.hpp"
#include "SpriteAnimationComponent.hpp"
#include "SpriteRenderComponent.hpp"

namespace Snowglobe::Render
{

class SpriteAnimationSystem : public Core::ECS::ISystem
{
  public:
    void Update() override
    {
        float deltaTime = Core::EngineTime::GetDeltaEngineFrameTime();
        for (auto& entity : _entityManager->GetAllEntities())
        {
            SpriteRenderComponent* sprite;
            SpriteAnimationComponent* animation;
            if (!entity->QueryComponents(sprite, animation))
                continue;

            animation->AnimationTickTimer -= deltaTime * animation->AnimationSpeed;
            if (animation->AnimationTickTimer > 0)
                continue;

            animation->AnimationTickTimer = animation->AnimationFrameRate;
            sprite->SpriteAsset.GetData().SetNextSprite();
        }
    }
};

} // namespace Snowglobe::Render