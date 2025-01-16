#include "Texture.hpp"

#include "glad/gl.h"

namespace Snowglobe::RenderOpenGL
{
    Texture2D::~Texture2D()
    {
        if (_initialized)
        {
            // glDeleteTextures(1, &_textureId);
        }
    }

    void Texture2D::Init(const uint8_t *data, uint32_t width, uint32_t height, uint32_t channels)
    {
        if (_initialized)
        {
            return;
        }

        _initialized = true;
        _width = width;
        _height = height;
        _channels = channels;

        int format = GetFormat(channels);

        glGenTextures(1, &_textureId);
        glBindTexture(GL_TEXTURE_2D, _textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetGLWrap(_uWrap));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetGLWrap(_vWrap));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLFiltering(_minFilter, true));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetGLFiltering(_magFilter, false));

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        if (_generateMipmaps)
            glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Texture2D::Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, _textureId);
    }

    int Texture2D::GetGLFiltering(Render::TextureFilter wrap, bool isMinFilter)
    {
        switch (wrap)
        {
        case Render::TextureFilter::Nearest:
            return isMinFilter ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
        case Render::TextureFilter::Linear:
            return isMinFilter ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
        default:
            return isMinFilter ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
        }
    }

    int Texture2D::GetGLWrap(Render::TextureWrap wrap)
    {
        switch (wrap)
        {
        case Render::TextureWrap::Repeat:
            return GL_REPEAT;
        case Render::TextureWrap::MirroredRepeat:
            return GL_MIRRORED_REPEAT;
        case Render::TextureWrap::ClampToEdge:
            return GL_CLAMP_TO_EDGE;
        case Render::TextureWrap::ClampToBorder:
            return GL_CLAMP_TO_BORDER;
        default:
            return GL_REPEAT;
        }
    }

    int Texture2D::GetFormat(uint32_t channels)
    {
        switch (channels)
        {
        case 1:
            return GL_RED;
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
        default:
            return GL_RGBA;
        }
    }
} // namespace Snowglobe::RenderOpenGL
