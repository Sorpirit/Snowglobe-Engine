#pragma once
#include "MeshOpenGL.hpp"
#include "SceneParameters.hpp"
#include "ECS/ISystem.hpp"
#include "Materials/BasicShapeMaterialImpl.hpp"


namespace Snowglobe::RenderOpenGL
{

class Shape2DSystem : public SnowCore::ECS::ISystem
{
public:
    Shape2DSystem(SnowCore::ECS::EntityManagerBase& entityManager);

    void Init();
    
    void Update() override;

private:
    void GenerateVertexBuffers();
    
    std::vector<MeshOpenGL*> _mesh;
    Materials::BasicShapeMaterialImpl _material;
    SceneParameters _sceneParameters;
    uint32_t _shaderProgram;
};
    
} // namespace Snowglobe::RenderOpenGL
