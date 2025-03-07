#pragma once

#include "glm/vec2.hpp"

#include <filesystem>

namespace Snowglobe::Core
{

class FileTexture
{
  public:
    FileTexture(const std::filesystem::path& path);
    ~FileTexture();

    uint8_t* GetData() const;

    glm::ivec2 Size() const { return {_width, _height}; }
    uint32_t GetWidth() const { return _width; }
    uint32_t GetHeight() const { return _height; }
    uint32_t GetChannels() const { return _channels; }

  private:
    uint8_t* _data;

    uint32_t _width;
    uint32_t _height;
    uint32_t _channels;
};

} // namespace Snowglobe::Core
