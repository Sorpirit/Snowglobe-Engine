#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include <array>

namespace Snowglobe::Render
{

class GizmosAPI
{
public:
    void AddTriangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec4 color) {
        PushVertex(v1, color, true);
        PushVertex(v2, color, true);
        PushVertex(v3, color, true);
    }
    void AddLine(glm::vec3 v1, glm::vec3 v2, glm::vec4 color)
    {
        PushVertex(v1, color, false);
        PushVertex(v2, color, false);
    }

    void AddTriangleWire(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec4 color) {
        AddLine(v1, v2, color);
        AddLine(v2, v3, color);
        AddLine(v3, v1, color);
    }

    void AddQuad(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4, glm::vec4 color)
    {
        AddTriangle(v1, v2, v3, color);
        AddTriangle(v1, v3, v4, color);
    }
    void AddQuadWire(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4, glm::vec4 color)
    {
        AddLine(v1, v2, color);
        AddLine(v2, v3, color);
        AddLine(v3, v4, color);
        AddLine(v4, v1, color);
        AddLine(v1, v3, color);
    }

    void AddQuad(glm::vec3 center, glm::vec3 halfSize, glm::vec4 color)
    {
        AddQuad(
            center + halfSize * glm::vec3(1, 1, 1),
            center + halfSize * glm::vec3(1, -1, 1),
            center + halfSize * glm::vec3(-1, -1, 1),
            center + halfSize * glm::vec3(-1, 1, 1),
            color);
    }
    void AddQuadWire(glm::vec3 center, glm::vec3 halfSize, glm::vec4 color)
    {
        AddQuadWire(
            center + halfSize * glm::vec3(1, 1, 1),
            center + halfSize * glm::vec3(1, -1, 1),
            center + halfSize * glm::vec3(-1, -1, 1),
            center + halfSize * glm::vec3(-1, 1, 1),
            color);
    }

    void AddVector(glm::vec3 from, glm::vec3 to, glm::vec3 cameraUp, glm::vec4 color)
    {
        glm::vec3 direction = glm::normalize(to - from);
        glm::vec3 forward = glm::cross(direction, cameraUp);
        glm::vec3 side = glm::cross(direction, forward);
        AddLine(from, to , color);
        AddLine(to + side * 0.1f - direction * 0.3f, to , color);
        AddLine(to - side * 0.1f - direction * 0.3f, to , color);
    }

    void AddAABB(glm::vec3 center, glm::vec3 halfSize, glm::vec4 color)
    {
        glm::vec3 a = center + halfSize * glm::vec3(1.0, 1.0, 1.0);
        glm::vec3 b = center + halfSize * glm::vec3(-1.0, 1.0, 1.0);
        glm::vec3 c = center + halfSize * glm::vec3(1.0, 1.0, -1.0);
        glm::vec3 d = center + halfSize * glm::vec3(-1.0, 1.0, -1.0);
        glm::vec3 a1 = center + halfSize * glm::vec3(1.0, -1.0, 1.0);
        glm::vec3 b1 = center + halfSize * glm::vec3(-1.0, -1.0, 1.0);
        glm::vec3 c1 = center + halfSize * glm::vec3(1.0, -1.0, -1.0);
        glm::vec3 d1 = center + halfSize * glm::vec3(-1.0, -1.0, -1.0);

        AddQuad(a, b, c, d, color);
        AddQuad(d1, c1, b1, a1, color);
    }

    void AddDisk(glm::vec3 center, float radius, glm::vec4 color)
    {
        for (int i = 0; i < _diskVertices.size(); ++i)
        {
            PushVertex(_diskVertices[i] * radius + center, color);
        }
    }

    void AddDiskWire(glm::vec3 center, float radius, glm::vec4 color)
    {
        for (int i = 0; i < _diskVerticesWire.size(); ++i)
        {
            PushVertex(_diskVerticesWire[i] * radius + center, color, false);
        }
    }
protected:
    virtual void PushVertex(glm::vec3 v, glm::vec4 color, bool fill = true) = 0;

    void CreateDiskVertices()
    {
        int segments = _diskVertices.size();
        float angle = 0.0f;
        float angleStep = glm::two_pi<float>() / static_cast<float>(segments / 3);
        for (int i = 0; i < segments; i+=3)
        {
            angle += angleStep;
            _diskVertices[i + 0] = {glm::vec3(0.0f, 0.0f, 0.0f)};
            _diskVertices[i + 1] = {glm::vec3(glm::cos(angle), glm::sin(angle), 0.0f)};
            _diskVertices[i + 2] = {glm::vec3(glm::cos(angle - angleStep), glm::sin(angle - angleStep), 0.0f)};
        }
    }

    void CreateDiskVerticesWire()
    {
        int segments = _diskVerticesWire.size() - 2;
        float angle = 0.0f;
        float angleStep = glm::two_pi<float>() / static_cast<float>(segments / 2);
        for (int i = 0; i < segments; i+=2)
        {
            angle += angleStep;
            _diskVerticesWire[i + 0] = {glm::vec3(glm::cos(angle), glm::sin(angle), 0.0f)};
            _diskVerticesWire[i + 1] = {glm::vec3(glm::cos(angle - angleStep), glm::sin(angle - angleStep), 0.0f)};
        }

        _diskVerticesWire[segments + 0] = {glm::vec3(glm::cos(0), glm::sin(0), 0.0f)};
        _diskVerticesWire[segments + 1] = {glm::vec3(0, 0, 0.0f)};
    }
private:
    std::array<glm::vec3, 96> _diskVertices;
    std::array<glm::vec3, 64> _diskVerticesWire;
};

} // namespace Snowglobe::Render