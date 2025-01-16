#pragma once 

#include <filesystem>

namespace Snowglobe::SnowCore
{
    class FileTexture
    {
    public:
        FileTexture(const std::filesystem::path& path);
        ~FileTexture();

        uint8_t* GetData() const;

        uint32_t GetWidth() const { return _width; }
        uint32_t GetHeight() const { return _height; }
        uint32_t GetChannels() const { return _channels; }

    private:
        uint8_t* _data;

        uint32_t _width;
        uint32_t _height;
        uint32_t _channels;
    };
    
} // namespace Snowglobe::SnowCore
