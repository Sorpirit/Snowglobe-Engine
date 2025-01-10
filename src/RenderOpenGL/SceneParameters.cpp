#include "SceneParameters.hpp"

#include <glad/gl.h>
namespace Snowglobe::RenderOpenGL
{

void SceneParameters::Bind(const Render::Camera& camera, uint32_t pipelineId)
{
    if(_lastPipelineId != pipelineId)
    {
        _cachedUniformLocationViewProjection = glGetUniformLocation(pipelineId, "sceneParameters.viewProjection");
        _lastPipelineId = pipelineId;
    }

    glm::mat4 viewProjection = camera.GetViewProjectionMatrix();
    glUniformMatrix4fv(_cachedUniformLocationViewProjection, 1, GL_FALSE, &viewProjection[0][0]);
}

} // namespace Snowglobe::RenderOpenGL


