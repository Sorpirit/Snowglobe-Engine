#pragma once
#include <nlohmann/json.hpp>

#include "ECS/Component.hpp"

#include "Assets/AssetManager.hpp"
#include "TextureAssetData.hpp"
namespace Snowglobe::Render
{

struct SpriteAssetData
{
public:
    Core::Asset<TextureAssetData> TextureAsset;

    //Sprite sheet info
    [[nodiscard]] uint32_t GetSpriteIndex() const { return CurrentSpriteCoord.x + CurrentSpriteCoord.y * TileCount.x; }
    [[nodiscard]] Texture2DPtr GetTexture() { return TextureAsset.GetData().RenderTexture; }

    void SetSpriteCoord(glm::ivec2 newSpriteCoord) { CurrentSpriteCoord = newSpriteCoord; }
    void SetSpriteIndex(uint32_t newSpriteIndex) { CurrentSpriteCoord = {newSpriteIndex % TileCount.x, newSpriteIndex / TileCount.x}; }
    void SetNextSprite()
    {
        CurrentSpriteCoord.x++;
        if (CurrentSpriteCoord.x < TileCount.x)
        {
            uint32_t spriteIndex = GetSpriteIndex();
            if (spriteIndex >= SpriteTotalCount)
            {
                CurrentSpriteCoord.x = 0;
                CurrentSpriteCoord.y = 0;
            }

            return;
        }

        CurrentSpriteCoord.x = 0;
        CurrentSpriteCoord.y++;
        if (CurrentSpriteCoord.y < TileCount.y)
            return;

        CurrentSpriteCoord.y = 0;
    }

    glm::vec2 TileSize = glm::ivec2(1);
    glm::ivec2 TileCount = glm::ivec2(1);
    uint32_t SpriteTotalCount = 1;
    glm::vec2 SpriteScale = glm::vec2(1);

    glm::ivec2 CurrentSpriteCoord = glm::ivec2(0);
};

class SpriteRenderComponent : public Core::ECS::Component
{
public:
    SpriteRenderComponent() = default;

    glm::vec3 Color = glm::vec3(1.0f);
    Core::AssetOptional<SpriteAssetData> SpriteAsset;
    bool flipX = false;
    bool flipY = false;
};
}