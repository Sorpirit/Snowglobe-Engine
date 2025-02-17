#include "SceneParameters.hpp"

#include <glad/gl.h>
namespace Snowglobe::RenderOpenGL
{

void SceneParameters::Bind(const Render::Camera& camera, uint32_t pipelineId)
{
    auto uniformSetter = _uniformLocations.GetSetter(pipelineId);
    uniformSetter.Set("sceneParameters.viewProjection", camera.GetViewProjectionMatrix());
    uniformSetter.Set("sceneParameters.viewPosition", camera.GetPosition());
    
}

} // namespace Snowglobe::RenderOpenGL


