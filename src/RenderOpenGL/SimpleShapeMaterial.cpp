#include "SimpleShapeMaterial.hpp"

#include <glad/gl.h>

namespace Snowglobe::RenderOpenGL
{

void SimpleShapeMaterial::Bind(uint32_t pipelineId)
{
    if(_lastPipelineId != pipelineId)
    {
        _cachedUniformLocationColor = glGetUniformLocation(pipelineId, "material.color");
        _lastPipelineId = pipelineId;
    }

    glUniform3fv(_cachedUniformLocationColor, 1, &_color[0]);
}
    
} // namespace Snowglobe::RenderOpenGL

