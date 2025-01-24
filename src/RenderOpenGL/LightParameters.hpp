#pragma once

#include <glm/glm.hpp>
#include "Camera.hpp"
#include "CachedUniformLocations.hpp"

#include "ECS/Component.hpp"

namespace Snowglobe::RenderOpenGL
{
    class LightParameters
    {
    public:
        void Bind(uint32_t pipelineId);

        glm::vec3 LightPosition = glm::vec3(0.0f);
        glm::vec3 LightColor = glm::vec3(1.0f);
        float AmbientIntensity = 0.1f;
    private:
        CachedUniformLocations<3> _uniformLocations;
    };


    const uint32_t LIGHT_TAG = 16; 
    
    class LightComponent : public SnowCore::ECS::Component
    {
    public:
        LightComponent() = default;
        LightComponent(const LightParameters& lightParameters) : _lightParameters(lightParameters) {}

        LightParameters& GetLightParameters() { return _lightParameters; }
    private:
        LightParameters _lightParameters;
    };
    
} // namespace Snowglobe::RenderOpenGL