#pragma once

#include <glm/glm.hpp>

namespace Snowglobe::Render
{
    struct RenderID
    {
        const uint32_t id;
    };

    class RenderProxy
    {
    public:
        virtual ~RenderProxy() = default;

    };
} // namespace Snowglobe::Render
