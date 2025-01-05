#pragma once

#include <string>

namespace Snowglobe::SnowCore
{
    enum class EngineRenderEngine
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
    

} // namespace Snowglobe::SnowCore
