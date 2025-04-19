#pragma once
#include "Serialization/SerializationAPI.hpp"
#include "SpriteAnimationComponent.hpp"
#include "SpriteRenderComponent.hpp"

namespace Snowglobe::Render
{

class RenderTypesSerialization
{
public:
    static void RegisterCustomTypes(Core::Serialization::SerializationAPI& api)
    {
        api.RegisterCustomProperty<SpriteAnimationComponent>(
            [](Core::Serialization::SerializationAPI* api, SpriteAnimationComponent& value) {
                api->Property("AnimationSpeed", value.AnimationSpeed);
                api->Property("AnimationTickTimer", value.AnimationTickTimer);
                return true;
            });

        api.RegisterCustomProperty<SpriteRenderComponent>(
            [](Core::Serialization::SerializationAPI* api, SpriteRenderComponent& value) {
                api->Property("Color", value.Color);
                api->Property("SpriteAsset", value.SpriteAsset.ToBase());
                return true;
            });
    }
};

} // namespace Snowglobe::Render