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
        uint32_t id;

        Texture2DPtr() : id(0) {}
        Texture2DPtr(uint32_t id) : id(id) {}

        
        static Texture2DPtr WhiteRBG () { return { 0 }; }
        static Texture2DPtr WhiteRBGA () { return { 1 }; }
        
        bool operator==(const Texture2DPtr& other) const { return id == other.id; }
        bool operator!=(const Texture2DPtr& other) const { return id != other.id; }

        operator uint32_t() const { return id; }
    };
}

template <> struct std::hash<Snowglobe::Render::Texture2DPtr>
{
    std::size_t operator()(const Snowglobe::Render::Texture2DPtr& texture) const
    {
        return std::hash<uint32_t>()(texture.id);
    }
};