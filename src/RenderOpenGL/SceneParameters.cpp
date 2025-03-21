#include "SceneParameters.hpp"

#include "UniformLocationSetter.hpp"

#include <glad/gl.h>
namespace Snowglobe::RenderOpenGL
{

void SceneParameters::Bind(const Render::Camera& camera, uint32_t pipelineId)
{
    UniformLocationSetter uniformSetter{pipelineId};
    uniformSetter.Set("sceneParameters.viewProjection", camera.GetViewProjectionMatrix());
    uniformSetter.Set("sceneParameters.viewPosition", camera.GetPosition());
    
}

} // namespace Snowglobe::RenderOpenGL


