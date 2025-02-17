#pragma once

#include <memory>
#include <unordered_map>
#include "Texture2DPtr.hpp"
#include "FileTexture.hpp"
#include "Texture.hpp"

namespace Snowglobe::RenderOpenGL
{
    class TextureManager
    {
    public:
        static TextureManager& GetInstance()
        {
            static TextureManager instance;
            return instance;
        }

        TextureManager();
        ~TextureManager();

        Render::Texture2DPtr CreateTexture2D(const Core::FileTexture& texture, const Render::Texture2DDescriptor& desc, const std::string& debugName = "Texture2D");

        Texture2D& GetTexture(Render::Texture2DPtr texturePtr)
        {
            auto it = _textures.find(texturePtr);
            if (it == _textures.end())
            {
                return _textures[_whiteRGB];
            }

            return it->second;
        }

    private:
        uint32_t _nextTextureId = 0;
        std::unordered_map<Render::Texture2DPtr, Texture2D> _textures;

        Render::Texture2DPtr _whiteRGB;
        Render::Texture2DPtr _whiteRGBA;
    };
    
} // namespace Snowglobe::RenderOpenGL
