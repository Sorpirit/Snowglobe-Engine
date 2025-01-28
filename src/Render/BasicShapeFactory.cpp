#include "BasicShapeFactory.hpp"
#include "CommonVertexLayouts.hpp"

namespace Snowglobe::Render
{

BasicShapeFactory::BasicShapeFactory(RenderSystem *renderSystem) : 
    _renderSystem(renderSystem)
{
    CreateTriangleVertices();
    CreatePlaneVertices();
    CreateDiskVertices();
}

BasicShapeFactory::~BasicShapeFactory()
{
    //todo: free vertex and index buffers
}

MeshProxy* BasicShapeFactory::CreateShape(BasicShape shape, const glm::vec3 &position,
                                                            const glm::vec3 &scale) const
{
    MeshProxy* mesh = nullptr;
    switch (shape)
    {
    case BasicShape::Triangle:
        mesh = _renderSystem->CreateMeshProxy(*_triangleVertexBuffer, *_triangleIndexBuffer, "Triangle");
        break;
    case BasicShape::Plane:
        mesh = _renderSystem->CreateMeshProxy(*_planeVertexBuffer, *_planeIndexBuffer, "Plane");
        break;
    case BasicShape::Disk:
        mesh = _renderSystem->CreateMeshProxy(*_diskVertexBuffer, *_diskIndexBuffer, "Disk");
        break;
    }

    if (mesh == nullptr)
        return nullptr;

    mesh->SetPosition(position);
    mesh->SetScale(scale);

    return mesh;
}

void BasicShapeFactory::CreateTriangleVertices()
{
    std::vector<PositionVertex> vertices = {
        {glm::vec3(0.5f, -0.5f, 0.0f)},
        {glm::vec3(-0.5f, -0.5f, 0.0f)},
        {glm::vec3(0.0f, 0.5f, 0.0f)}
    };

    std::vector<unsigned int> indices = {
        0, 1, 2
    };

    _triangleVertexBuffer = _renderSystem->AllocateVertexBufferPtr<PositionVertex>(vertices, "TriangleVertexBuffer");
    _triangleIndexBuffer = _renderSystem->AllocateIndexBufferPtr(indices, "TriangleIndexBuffer");
}

void BasicShapeFactory::CreatePlaneVertices()
{
    std::vector<PositionVertex> vertices = {
        {glm::vec3(-0.5f, -0.5f, 0.0f)},
        {glm::vec3(-0.5f, 0.5f, 0.0f)},
        {glm::vec3(0.5f, 0.5f, 0.0f)},
        {glm::vec3(0.5f, -0.5f, 0.0f)}
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3
    };

    _planeVertexBuffer = _renderSystem->AllocateVertexBufferPtr<PositionVertex>(vertices, "PlaneVertexBuffer");
    _planeIndexBuffer = _renderSystem->AllocateIndexBufferPtr(indices, "PlaneIndexBuffer");
}

void BasicShapeFactory::CreateDiskVertices()
{
    int segments = 32;
    float angle = 0.0f;
    float angleStep = glm::two_pi<float>() / static_cast<float>(segments);
    std::vector<PositionVertex> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back({glm::vec3(0.0f, 0.0f, 0.0f)});
    vertices.push_back({glm::vec3(0.5f, 0.0f, 0.0f)});
    for (int i = 2; i < segments + 1; i++)
    {
        angle += angleStep;
        vertices.push_back({glm::vec3(glm::cos(angle) / 2.0f, glm::sin(angle) / 2.0f, 0.0f)});
        indices.push_back(0);
        indices.push_back(i - 1);
        indices.push_back(i);
    }

    indices.push_back(0);
    indices.push_back(segments);
    indices.push_back(1);

    _diskVertexBuffer = _renderSystem->AllocateVertexBufferPtr<PositionVertex>(vertices, "DiskVertexBuffer");
    _diskIndexBuffer = _renderSystem->AllocateIndexBufferPtr(indices, "DiskIndexBuffer");
}
    
} // namespace Snowglobe::Render
