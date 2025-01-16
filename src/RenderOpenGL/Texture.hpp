#pragma once

#include "glm/glm.hpp"

#include "Texture2DPtr.hpp"

namespace Snowglobe::RenderOpenGL
{
    
    class Texture2D
    {
    public:
        Texture2D() = default;

        Texture2D(Render::TextureWrap uvWrap, Render::TextureFilter minFilter, Render::TextureFilter magFilter, bool generateMipmaps = true) :
            _uWrap(uvWrap), _vWrap(uvWrap), _minFilter(minFilter), _magFilter(magFilter), _generateMipmaps(generateMipmaps) {};

        Texture2D(Render::TextureWrap uWrap, Render::TextureWrap vWrap, Render::TextureFilter minFilter, Render::TextureFilter magFilter, bool generateMipmaps = true) :
            _uWrap(uWrap), _vWrap(vWrap), _minFilter(minFilter), _magFilter(magFilter), _generateMipmaps(generateMipmaps) {};

        ~Texture2D();

        void Init(const uint8_t* data, uint32_t width, uint32_t height, uint32_t channels);

        uint32_t GetWidth() const { return _width; }
        uint32_t GetHeight() const { return _height; }
        uint32_t GetChannels() const { return _channels; }

        void Bind() const;

    private:
        bool _initialized = false;
        uint32_t _textureId = UINT32_MAX;

        bool _generateMipmaps = true;

        uint32_t _width = UINT32_MAX;
        uint32_t _height = UINT32_MAX;
        uint32_t _channels = UINT32_MAX;

        Render::TextureWrap _uWrap;
        Render::TextureWrap _vWrap;
        Render::TextureFilter _minFilter;
        Render::TextureFilter _magFilter;

        static int GetGLFiltering(Render::TextureFilter wrap, bool isMinFilter);
        static int GetGLWrap(Render::TextureWrap wrap);

        int GetFormat(uint32_t channels);
    };

} // namespace Snowglobe::RenderOpenGL