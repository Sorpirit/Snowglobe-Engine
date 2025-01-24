#include "LightParameters.hpp"

#include <glad/gl.h>
namespace Snowglobe::RenderOpenGL
{

void LightParameters::Bind(uint32_t pipelineId)
{
    auto uniformSetter = _uniformLocations.GetSetter(pipelineId);
    uniformSetter.Set("pointLight.position", LightPosition);
    uniformSetter.Set("pointLight.color", LightColor);
    uniformSetter.Set("pointLight.ambientIntensity", AmbientIntensity);
}

} // namespace Snowglobe::RenderOpenGL


