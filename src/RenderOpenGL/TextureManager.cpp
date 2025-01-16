#include "TextureManager.hpp"

namespace Snowglobe::RenderOpenGL
{
    TextureManager::TextureManager()
    {
    }

    TextureManager::~TextureManager()
    {
    }

    Render::Texture2DPtr TextureManager::CreateTexture2D(const SnowCore::FileTexture &texture,
                                                         const Render::Texture2DDescriptor &desc,
                                                         const std::string &debugName)
    {
        auto val = _textures.emplace(_nextTextureId++, 
            Texture2D(desc.wrapU, desc.wrapV, desc.minFilter, desc.magFilter, desc.generateMipmaps));
        val.first->second.Init(texture.GetData(), texture.GetWidth(), texture.GetHeight(), texture.GetChannels());
        return val.first->first;
    }
}