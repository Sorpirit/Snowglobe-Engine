#pragma once
#include "../Core/Serialization/SerializationAPI.hpp"
#include "../Engine/Engine.hpp"
#include "ECS/Component.hpp"
#include "ECS/ECSTest/ECSTest.hpp"
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
    glm::ivec2 Size = glm::ivec2(0);
    glm::ivec2 TileSize = glm::ivec2(0);
    glm::ivec2 TileOffset = glm::ivec2(0);
    glm::ivec2 TileCount = glm::ivec2(1);
    uint32_t SpriteCount = 1;
    glm::vec2 TileSizeUnit() { return  glm::vec2(TileSize) / glm::vec2(Size); }
};
}

template <>
inline void CustomProp<Snowglobe::Render::SpriteRenderComponent>(Snowglobe::Core::Serialization::SerializationAPI* api,
                                                               Snowglobe::Render::SpriteRenderComponent* value, uint32_t metaFlags)
{
    api->Property("File", value->File);
    api->Property("Color", value->Color);
    api->Property("TileSize", value->TileSize);
    api->Property("TileOffset", value->TileOffset);
    api->BaseProperty("SpriteCount", value->SpriteCount);

    if (value->Sprite.ID == 0)
    {
        auto fsprite = DI->Resolve<Snowglobe::Core::FileSystem>()->LoadTexture(value->File);
        auto textureDesc = Snowglobe::Render::Texture2DDescriptor{
            Snowglobe::Render::TextureWrap::Repeat, Snowglobe::Render::TextureWrap::Repeat,
            Snowglobe::Render::TextureFilter::Nearest, Snowglobe::Render::TextureFilter::Nearest};

        Snowglobe::Render::RenderSystem* renderPtr;
        DI->Resolve<Snowglobe::Engine::Engine>()->GetSystemManager()->QuerySystem(renderPtr);
        value->Sprite = renderPtr->CreateTexture2D(*fsprite, textureDesc);

        value->Size.x = fsprite->GetWidth();
        value->Size.y = fsprite->GetHeight();
        value->Size.x = fsprite->GetWidth();
        value->Size.y = fsprite->GetHeight();
        if (value->TileSize.x == 0 || value->TileSize.y == 0)
        {
            value->TileSize = value->Size;
            value->TileOffset = glm::vec2(0, 0);
        }
        else
        {
            value->TileCount = value->Size / value->TileSize;
        }

    }
}