#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

#include "ECS/Component.hpp"

namespace Snowglobe::Core
{
    
class TransformComponent : public ECS::Component
{
public:
    TransformComponent() = default;
    TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : Position(position), Rotation(rotation), Scale(scale) {}

    glm::vec3 GetUp() const { return glm::eulerAngleXYZ(glm::radians(Rotation.x), glm::radians(Rotation.y), glm::radians(Rotation.z)) * glm::vec4(0, 1.0f, 0, 0.0f);  }
    glm::vec3 GetRight() const { return glm::eulerAngleXYZ(glm::radians(Rotation.x), glm::radians(Rotation.y), glm::radians(Rotation.z)) * glm::vec4(1.0f, 0, 0, 0.0f);  }
    glm::vec3 GetForward() const { return glm::eulerAngleXYZ(glm::radians(Rotation.x), glm::radians(Rotation.y), glm::radians(Rotation.z)) * glm::vec4(0, 0, 1.0f, 0.0f);  }
    
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Rotation = glm::vec3(0.0f);
    glm::vec3 Scale = glm::vec3(1.0f);
};

}
