#pragma once
#include "Assets/AssetManager.hpp"
#include "TextureAssetData.hpp"
namespace Snowglobe::Render
{

class SpriteRenderComponent : public Core::ECS::Component
{
public:
    SpriteRenderComponent() = default;

    Core::AssetOptional<TextureAssetData> SpriteAsset;
    glm::vec3 Color = glm::vec3(1.0f);

    //Sprite sheet values
    glm::ivec2 TileSize = glm::ivec2(0);
    glm::ivec2 TileOffset = glm::ivec2(0);
    glm::ivec2 TileCount = glm::ivec2(1);
    uint32_t SpriteCount = 1;
    glm::ivec2 Size() { return SpriteAsset.GetData().FiletTexture->Size(); }
    glm::vec2 TileSizeUnit() { return  glm::vec2(TileSize) / glm::vec2(Size()); }
};
}

template <>
inline void CustomProperty<Snowglobe::Render::SpriteRenderComponent>(Snowglobe::Core::Serialization::SerializationAPI* api,
                                                               Snowglobe::Render::SpriteRenderComponent* value, uint32_t metaFlags)
{
    api->Property("Color", value->Color);
    api->Property("SpriteAsset", value->SpriteAsset.ToBase());
    auto defaultSize = value->Size();
    api->Property("TileSize", value->TileSize, defaultSize);
    api->Property("TileOffset", value->TileOffset);
    api->BaseProperty("SpriteCount", value->SpriteCount);
}