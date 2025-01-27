#pragma once

namespace Snowglobe::Render
{
    enum class TextureWrap
    {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
    };

    enum class TextureFilter
    {
        Nearest,
        Linear,
        // Anisotropic,
    };


    struct Texture2DDescriptor
    {
        TextureWrap wrapU = TextureWrap::Repeat;
        TextureWrap wrapV = TextureWrap::Repeat;
        TextureFilter minFilter = TextureFilter::Linear;
        TextureFilter magFilter = TextureFilter::Linear;
        bool generateMipmaps = true;
    };

    struct Texture2DPtr
    {
        uint32_t ID;

        Texture2DPtr() : ID(0) {}
        Texture2DPtr(uint32_t id) : ID(id) {}
        
        bool operator==(const Texture2DPtr& other) const { return ID == other.ID; }
        bool operator!=(const Texture2DPtr& other) const { return ID != other.ID; }

        operator uint32_t() const { return ID; }
    };
}

template <> struct std::hash<Snowglobe::Render::Texture2DPtr>
{
    std::size_t operator()(const Snowglobe::Render::Texture2DPtr& texture) const noexcept
    {
        return std::hash<uint32_t>()(texture.ID);
    }
};