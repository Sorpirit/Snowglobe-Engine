#pragma once
#include "../Core/Serialization/SerializationAPI.hpp"
#include "ECS/Component.hpp"

namespace Snowglobe::Render
{

struct AnimationClip
{
    bool Started = false;
    bool Finished = false;
    glm::ivec2 StartPosition = glm::ivec2(0);
    glm::ivec2 EndPosition = glm::ivec2(0);
    bool Loop = false;
    float AnimationDuration = 1.0f / 24.0f;

    AnimationClip() = default;
    AnimationClip(const glm::ivec2& start_position, const glm::ivec2& end_position, bool loop, float animation_duration)
        : StartPosition(start_position), EndPosition(end_position), Loop(loop), AnimationDuration(animation_duration)
    {
    }
};

class SpriteAnimationComponent : public Core::ECS::Component
{
  public:
    SpriteAnimationComponent() = default;

    std::unordered_map<uint32_t, AnimationClip> AnimationClips;
    AnimationClip Clip;
    float AnimationSpeed = 1.0f;
    float AnimationTickTimer = 0.0f;

    void SetAnimationIndex(uint32_t animationIndex)
    {
        _currentAnimationIndex = animationIndex;
        _animationIndexDirty = true;
    }
    uint32_t GetAnimationIndex() const { return _currentAnimationIndex; }

    bool AnimationIndexChanged() const { return _animationIndexDirty; }
    void ResetAnimationChangedFlag() { _animationIndexDirty = false; }

  private:
    bool _animationIndexDirty = false;
    uint32_t _currentAnimationIndex = 0;
};

} // namespace Snowglobe::Render