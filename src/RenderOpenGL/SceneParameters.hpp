#pragma once

#include <glm/glm.hpp>
#include "Camera.hpp"
#include "CachedUniformLocations.hpp"

namespace Snowglobe::RenderOpenGL
{
    class SceneParameters
    {
    public:
        void Bind(const Render::Camera& camera, uint32_t pipelineId);
        
    private:
        CachedUniformLocations<1> _uniformLocations;
    };
    
} // namespace Snowglobe::RenderOpenGL