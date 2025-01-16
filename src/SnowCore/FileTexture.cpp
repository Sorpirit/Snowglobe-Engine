#include "FileTexture.hpp"

#include "stb_image.hpp"

namespace Snowglobe::SnowCore
{
    FileTexture::FileTexture(const std::filesystem::path& path)
    {
        int width, height, channels;
        _data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

        _width = width;
        _height = height;
        _channels = channels;
    }

    FileTexture::~FileTexture()
    {
        stbi_image_free(_data);
    }

    uint8_t* FileTexture::GetData() const
    {
        return _data;
    }


} // namespace Snowglobe::SnowCore