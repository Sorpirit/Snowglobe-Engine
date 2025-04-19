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

            auto& spriteData = sprite->SpriteAsset.GetData();

            //Animation State machine
            auto it = animation->AnimationStateMachine.find(animation->GetAnimationIndex());
            if (it != animation->AnimationStateMachine.end())
            {
                for (auto& [toState, condition] : it->second)
                {
                    bool transition = condition();
                    if (transition)
                    {
                        animation->SetAnimationIndex(toState);
                        break;
                    }
                }
            }


            if (animation->AnimationIndexChanged())
            {
                animation->ResetAnimationChangedFlag();
                animation->Clip = animation->AnimationClips[animation->GetAnimationIndex()];
            }

            if (!animation->Clip.Started)
            {
                spriteData.SetSpriteCoord(animation->Clip.StartPosition);
                animation->Clip.Started = true;
                continue;
            }

            if (animation->Clip.Finished)
                continue;

            animation->AnimationTickTimer -= deltaTime * animation->AnimationSpeed;
            if (animation->AnimationTickTimer > 0)
                continue;

            animation->AnimationTickTimer = animation->Clip.AnimationDuration;
            if (animation->Clip.EndPosition == spriteData.CurrentSpriteCoord)
            {
                if (animation->Clip.Loop)
                    spriteData.SetSpriteCoord(animation->Clip.StartPosition);
                else
                    animation->Clip.Finished = true;

                continue;
            }

            spriteData.SetNextSprite();
        }
    }
};

} // namespace Snowglobe::Render