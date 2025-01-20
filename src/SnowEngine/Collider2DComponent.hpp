#pragma once

#include "ECS/Component.hpp"

namespace Snowglobe::SnowEngine
{

enum class CollisionShapeType
{
    Circle,
    AABB
};

class Collider2DComponent : public SnowCore::ECS::Component
{
public:
    Collider2DComponent() = default;
    Collider2DComponent(CollisionShapeType shapeType) : ShapeType(shapeType) {}
    
    CollisionShapeType ShapeType;
};

}
