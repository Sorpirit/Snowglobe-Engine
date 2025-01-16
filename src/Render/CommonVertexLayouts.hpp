#pragma once

#include <glm/glm.hpp>

namespace Snowglobe::Render
{
    struct PositionVertex
    {
        glm::vec3 position = glm::vec3(0.0f);
    };

    struct PositionColorVertex
    {
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec4 color = glm::vec4(1.0f);
    };

    struct PositionUVVertex
    {
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec2 uv = glm::vec2(0.0f);
    };

    struct PositionNormalUVVertex
    {
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 normal = glm::vec3(0.0f);
        glm::vec2 uv = glm::vec2(0.0f);
    };

    struct PositionNormalTangentUVVertex
    {
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 normal = glm::vec3(0.0f);
        glm::vec3 tangent = glm::vec3(0.0f);
        glm::vec2 uv = glm::vec2(0.0f);
    };

} // namespace Snowglobe::Render