#pragma once
#include "ECS/Component.hpp"
#include "FileSystem.hpp"
#include "Texture2DPtr.hpp"

namespace Snowglobe::Render
{

class SpriteRenderComponent : public Core::ECS::Component
{
public:
    SpriteRenderComponent() = default;

    Core::SnowFileHandle File = "";
    glm::vec3 Color = glm::vec3(1.0f);
    Texture2DPtr Sprite;
};

}