#pragma once

#include <glm/glm.hpp>
#include "Camera.hpp"
#include "CachedUniformLocations.hpp"
#include "ECS/Tag.hpp"

#include "ECS/Component.hpp"

REGISTER_TAG(Lights)

namespace Snowglobe::RenderOpenGL
{
    
// WARNING! Should be the same as in "textureLit.frag"
constexpr size_t POINT_LIGHTS_COUNT = 4;
constexpr size_t SPOT_LIGHTS_COUNT = 4;

struct LightParameters
{
    uint32_t PointLightsCount;
    uint32_t SpotLightsCount;
};
    
struct DirectionalLight
{
    glm::vec3 LightDirection = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 LightColor = glm::vec3(1.0f);
    float AmbientIntensity = 0.1f;
    float EnvIntensity = 0.65f;
    int UseEnvMapping = 1;
};

struct PointLight
{
    glm::vec3 LightPosition = glm::vec3(0.0f);
    glm::vec3 LightColor = glm::vec3(1.0f);
    float AmbientIntensity = 0.1f;
    glm::vec3 AttenuationCoefficients = glm::vec3(1.0, 0.22, 0.20);
    float MaxDistance = 25.0f;
};

struct SpotLight
{
    glm::vec3 LightPosition = glm::vec3(0.0f);
    glm::vec3 LightDirection = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 LightColor = glm::vec3(1.0f);
    float AmbientIntensity = 0.1f;
    float InnerCutoffAngle = glm::radians(30.0f);
    float OuterCutoffAngle = glm::radians(60.0f);
    glm::vec3 AttenuationCoefficients = glm::vec3(1.0, 0.22, 0.20);
    float MaxDistance = 25.0f;
};

class LightsManager
{
public:
    void Bind(uint32_t pipelineId);

    void Reset()
    {
        _pointLightCount = 0;
        _spotLightCount = 0;
    }

    DirectionalLight& GetDirectionalLight() { return _directional; }
    PointLight& NextPointLight(){ return _pointLights[_pointLightCount++]; }
    SpotLight& NextSpotLight(){ return _spotLights[_spotLightCount++]; }
    
    
private:
    DirectionalLight _directional;
    std::array<PointLight, POINT_LIGHTS_COUNT> _pointLights;
    std::array<SpotLight, SPOT_LIGHTS_COUNT> _spotLights;

    uint32_t _pointLightCount;
    uint32_t _spotLightCount;
    
    CachedUniformLocations<7 + POINT_LIGHTS_COUNT * 5 + SPOT_LIGHTS_COUNT * 8> _uniformLocations;
};
    
class DirectionalLightComponent : public Core::ECS::Component
{
public:
    DirectionalLightComponent() = default;
    DirectionalLightComponent(const DirectionalLight& light) : Light(light) {}

    DirectionalLight Light;
};
class PointLightComponent : public Core::ECS::Component
{
public:
    PointLightComponent() = default;
    PointLightComponent(const PointLight& light) : Light(light) {}
    
    PointLight Light;
};
class SpotLightComponent : public Core::ECS::Component
{
public:
    SpotLightComponent() = default;
    SpotLightComponent(const SpotLight& light) : Light(light) {}

    SpotLight Light;
};
    
} // namespace Snowglobe::RenderOpenGL