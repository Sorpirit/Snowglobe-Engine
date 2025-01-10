#pragma once

#include <glm/glm.hpp>
#include "Camera.hpp"

namespace Snowglobe::RenderOpenGL
{
    class SceneParameters
    {
    public:
        void Bind(const Render::Camera& camera, uint32_t pipelineId);
        
    private:
        uint32_t _lastPipelineId = UINT_MAX;
        uint32_t _cachedUniformLocationViewProjection = UINT_MAX;
    };
    
} // namespace Snowglobe::RenderOpenGL