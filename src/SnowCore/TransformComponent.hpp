#pragma once

#include <glm/glm.hpp>

#include "ECS/Component.hpp"

namespace Snowglobe::SnowCore
{
    
class TransformComponent : public ECS::Component
{
public:
    TransformComponent() = default;
    TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : Position(position), Rotation(rotation), Scale(scale) {}
    
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Rotation = glm::vec3(0.0f);
    glm::vec3 Scale = glm::vec3(1.0f);
};

} // namespace Snowglobe::SnowCore
