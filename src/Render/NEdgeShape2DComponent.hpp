#pragma once

#include "ECS/Component.hpp"

namespace Snowglobe::Render
{
    
class NEdgeShape2DComponent : public Core::ECS::Component
{
public:
    NEdgeShape2DComponent() = default;
    NEdgeShape2DComponent(int edgeCount) : EdgeCount(edgeCount) {}
    NEdgeShape2DComponent(int edgeCount, glm::vec3 color) : EdgeCount(edgeCount), Color(color) {}
    NEdgeShape2DComponent(int edgeCount, glm::vec3 color, glm::vec3 outlineColor) : EdgeCount(edgeCount), Color(color), UseOutline(true), Outline(outlineColor) {}

    int EdgeCount;
    glm::vec3 Color = glm::vec3(1.0f);
    bool UseOutline = false;
    float OutlineWidth = 0.05f;
    glm::vec3 Outline = glm::vec3(1.0f);
    float Alpha = 1.0f;
};
    
} // namespace Snowglobe::Render
