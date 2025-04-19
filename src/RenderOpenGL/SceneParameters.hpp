#pragma once

#include "Camera.hpp"

namespace Snowglobe::RenderOpenGL
{
    class SceneParameters
    {
    public:
        void Bind(const Render::Camera& camera, uint32_t pipelineId);
    };
    
} // namespace Snowglobe::RenderOpenGL