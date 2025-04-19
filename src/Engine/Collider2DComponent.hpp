#pragma once

#include "Serialization/SerializationAPI.hpp"
#include "ECS/Component.hpp"

namespace Snowglobe::Engine
{

enum class CollisionShapeType : uint8_t
{
    Circle,
    AABB
};

struct CollisionData
{
    bool IsColliding = false;
    glm::vec2 Normal = glm::vec2(0, 0);
    float Penetration = 0.0f;
};

struct EntityCollisionData
{
    uint64_t LastFrameN = 0;
    bool IsColliding = false;
    std::weak_ptr<Core::ECS::Entity> Other;
    Core::ECS::Tag OtherTag = Tags::Default();
    CollisionData Data;
};

class Collider2DComponent : public Core::ECS::Component
{
  public:
    Collider2DComponent() = default;
    Collider2DComponent(CollisionShapeType shapeType, bool isTrigger = false)
        : ShapeType(shapeType), IsTrigger(isTrigger)
    {
    }

    CollisionShapeType ShapeType;
    bool IsTrigger = false;
    EntityCollisionData CollisionData;
};

} // namespace Snowglobe::Engine