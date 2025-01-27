#pragma once

#include "ECS/Component.hpp"

namespace Snowglobe::Render
{
    
class NEdgeShape2DComponent : public Core::ECS::Component
{
public:
    NEdgeShape2DComponent() = default;
    NEdgeShape2DComponent(int edgeCount) : EdgeCount(edgeCount) {}

    int EdgeCount;
    glm::vec3 Color = glm::vec3(1.0f);
};
    
} // namespace Snowglobe::Render
