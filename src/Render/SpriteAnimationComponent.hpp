#pragma once
#include "../Core/Serialization/SerializationAPI.hpp"
#include "ECS/Component.hpp"

namespace Snowglobe::Render
{

class SpriteAnimationComponent : public Core::ECS::Component
{
public:
    SpriteAnimationComponent() = default;

    float AnimationSpeed = 1.0f;
    float AnimationTickTimer = 0.0f;
    float AnimationFrameRate = 1.0f / 24.0f;
};

}


template <>
inline void CustomProperty<Snowglobe::Render::SpriteAnimationComponent>(Snowglobe::Core::Serialization::SerializationAPI* api,
                                                               Snowglobe::Render::SpriteAnimationComponent* value, uint32_t metaFlags)
{
    api->BaseProperty("AnimationSpeed", value->AnimationSpeed, metaFlags);
    api->BaseProperty("AnimationTickTimer", value->AnimationTickTimer, metaFlags);
    api->BaseProperty("AnimationFrameRate", value->AnimationFrameRate, metaFlags);
}