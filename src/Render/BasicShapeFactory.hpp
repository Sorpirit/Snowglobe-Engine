#pragma once

#include <glm/glm.hpp>

#include "RenderSystem.hpp"
#include "RenderProxy.hpp"
#include "VertexBufferPtr.hpp"
#include "IndexBufferPtr.hpp"
#include "MeshProxy.hpp"

namespace Snowglobe::Render
{
    enum class BasicShape
    {
        Triangle,
        Plane,
        Disk
    };

    struct BasicShapeMaterial
    {
        glm::vec3 color = glm::vec3(1.0f);
    };

    class BasicShapeFactory
    {
    public:
        BasicShapeFactory(RenderSystem* renderSystem);
        ~BasicShapeFactory();

        MeshProxy* CreateShape(BasicShape shape, const glm::vec3& position, const glm::vec3& scale) const;
    private:
        RenderSystem* _renderSystem;

        VertexBufferPtr* _triangleVertexBuffer = nullptr;
        IndexBufferPtr* _triangleIndexBuffer = nullptr;

        VertexBufferPtr* _planeVertexBuffer = nullptr;
        IndexBufferPtr* _planeIndexBuffer = nullptr;

        VertexBufferPtr* _diskVertexBuffer = nullptr;
        IndexBufferPtr* _diskIndexBuffer = nullptr;

        void CreateTriangleVertices();
        void CreatePlaneVertices();
        void CreateDiskVertices();

    };

} // namespace Snowglobe::Render
