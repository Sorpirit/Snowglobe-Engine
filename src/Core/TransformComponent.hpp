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
    TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
        : Position(position), Rotation(rotation), Scale(scale)
    {
    }

    [[nodiscard]] auto GetUp() const -> glm::vec3
    {
        return glm::eulerAngleXYZ(glm::radians(Rotation.x), glm::radians(Rotation.y), glm::radians(Rotation.z)) *
               glm::vec4(0, 1.0F, 0, 0);
    }
    [[nodiscard]] auto GetRight() const -> glm::vec3
    {
        return glm::eulerAngleXYZ(glm::radians(Rotation.x), glm::radians(Rotation.y), glm::radians(Rotation.z)) *
               glm::vec4(1.0F, 0, 0, 0);
    }
    [[nodiscard]] auto GetForward() const -> glm::vec3
    {
        return glm::eulerAngleXYZ(glm::radians(Rotation.x), glm::radians(Rotation.y), glm::radians(Rotation.z)) *
               glm::vec4(0, 0, 1.0F, 0);
    }

    glm::vec3 Position = glm::vec3(0);
    glm::vec3 Rotation = glm::vec3(0);
    glm::vec3 Scale = glm::vec3(1);
};

} // namespace Snowglobe::Core
