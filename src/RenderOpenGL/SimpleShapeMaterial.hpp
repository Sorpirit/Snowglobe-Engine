#pragma once

#include <glm/glm.hpp>

namespace Snowglobe::RenderOpenGL
{
    class SimpleShapeMaterial
    {
    public:
        void SetColor(const glm::vec3& color) { _color = color; }

        void Bind(uint32_t pipelineId);

    private:
        glm::vec3 _color = glm::vec3(1.0f);

        uint32_t _lastPipelineId = UINT_MAX;
        uint32_t _cachedUniformLocationColor = UINT_MAX;
    };
    
} // namespace Snowglobe::RenderOpenGL
