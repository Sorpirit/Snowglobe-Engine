#pragma once

#include <string>

namespace Snowglobe::Core
{

enum class EngineRenderEngine : uint8_t
{
    OpenGL,
    Vulkan,
    DirectX
};

struct EngineProfile
{
    std::string applicationName;
    EngineRenderEngine preferredRenderEngine;
};

} // namespace Snowglobe::Core
