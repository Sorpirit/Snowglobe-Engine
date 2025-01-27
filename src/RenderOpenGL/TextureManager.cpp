#include "TextureManager.hpp"

namespace Snowglobe::RenderOpenGL
{
    TextureManager::TextureManager()
    {
        // Create a white texture
        uint8_t white[3] = { 255, 255, 255 };
        Render::Texture2DDescriptor desc;
        auto whiteRGB = _textures.emplace(_nextTextureId++, 
            Texture2D(desc.wrapU, desc.wrapV, desc.minFilter, desc.magFilter, desc.generateMipmaps));
        whiteRGB.first->second.Init(white, 1, 1, 3);
        _whiteRGB = whiteRGB.first->first;

        uint8_t whiteAlpha[4] = { 255, 255, 255, 255 };
        auto whiteRGBA = _textures.emplace(_nextTextureId++, 
            Texture2D(desc.wrapU, desc.wrapV, desc.minFilter, desc.magFilter, desc.generateMipmaps));
        whiteRGBA.first->second.Init(whiteAlpha, 1, 1, 4);
        _whiteRGBA = whiteRGBA.first->first;
    }

    TextureManager::~TextureManager()
    {
    }

    Render::Texture2DPtr TextureManager::CreateTexture2D(const Core::FileTexture &texture,
                                                         const Render::Texture2DDescriptor &desc,
                                                         const std::string &debugName)
    {
        auto val = _textures.emplace(_nextTextureId++, 
            Texture2D(desc.wrapU, desc.wrapV, desc.minFilter, desc.magFilter, desc.generateMipmaps));
        val.first->second.Init(texture.GetData(), texture.GetWidth(), texture.GetHeight(), texture.GetChannels());
        return val.first->first;
    }
}