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

            sprite->TileOffset.x++;
            if (sprite->TileOffset.x < sprite->TileCount.x)
            {
                int spriteIndex = sprite->TileOffset.x + sprite->TileOffset.y * sprite->TileCount.x;
                if (spriteIndex >= sprite->SpriteCount)
                {
                    sprite->TileOffset.x = 0;
                    sprite->TileOffset.y = 0;
                }

                continue;
            }

            sprite->TileOffset.x = 0;
            sprite->TileOffset.y++;
            if (sprite->TileOffset.y < sprite->TileCount.y)
                continue;

            sprite->TileOffset.y = 0;
        }
    }
};

} // namespace Snowglobe::Render