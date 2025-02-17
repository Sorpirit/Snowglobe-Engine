#include "LightsManager.hpp"

#include <glad/gl.h>
namespace Snowglobe::RenderOpenGL
{

void LightsManager::Bind(uint32_t pipelineId)
{
    auto uniformSetter = _uniformLocations.GetSetter(pipelineId);

    //LightParameters
    uniformSetter.Set("lightParameters.pointLightsCount", _pointLightCount);
    uniformSetter.Set("lightParameters.spotLightsCount", _spotLightCount);

    //DirectionalLight
    uniformSetter.Set("directionalLight.direction", _directional.LightDirection);
    uniformSetter.Set("directionalLight.color", _directional.LightColor);
    uniformSetter.Set("directionalLight.ambientIntensity", _directional.AmbientIntensity);

    //PointLight
    for (uint32_t i = 0; i < POINT_LIGHTS_COUNT; ++i)
    {
        auto& pointLight = _pointLights[i];
        uniformSetter.Set(std::format("pointLights[{}].position", i), pointLight.LightPosition);
        uniformSetter.Set(std::format("pointLights[{}].color", i), pointLight.LightColor);
        uniformSetter.Set(std::format("pointLights[{}].ambientIntensity", i), pointLight.AmbientIntensity);
        uniformSetter.Set(std::format("pointLights[{}].attenuationCoefficients", i), pointLight.AttenuationCoefficients);
        uniformSetter.Set(std::format("pointLights[{}].maxDistance", i), pointLight.MaxDistance);
    }

    //SpotLight
    for (uint32_t i = 0; i < SPOT_LIGHTS_COUNT; ++i)
    {
        auto& spotLight = _spotLights[i];
        uniformSetter.Set(std::format("spotLights[{}].position", i), spotLight.LightPosition);
        uniformSetter.Set(std::format("spotLights[{}].direction", i), spotLight.LightDirection);
        uniformSetter.Set(std::format("spotLights[{}].color", i), spotLight.LightColor);
        uniformSetter.Set(std::format("spotLights[{}].ambientIntensity", i), spotLight.AmbientIntensity);
        uniformSetter.Set(std::format("spotLights[{}].innerCutoffAngle", i), spotLight.InnerCutoffAngle * 0.5f);
        uniformSetter.Set(std::format("spotLights[{}].outerCutoffAngle", i), spotLight.OuterCutoffAngle * 0.5f);
        uniformSetter.Set(std::format("spotLights[{}].attenuationCoefficients", i), spotLight.AttenuationCoefficients);
        uniformSetter.Set(std::format("spotLights[{}].maxDistance", i), spotLight.MaxDistance);
    }
}

} // namespace Snowglobe::RenderOpenGL


