#pragma once
#include "ECS/ISystem.hpp"
#include "Materials/BasicShapeMaterialImpl.hpp"
#include "MeshOpenGL.hpp"
#include "SceneParameters.hpp"
#include "ShaderCompiler.hpp"

#include <complex.h>

namespace Snowglobe::RenderOpenGL
{

class Shape2DSystem : public Core::ECS::ISystem
{
public:
    void Init(std::shared_ptr<Core::ECS::EntityManagerBase> entityManager) override;
    
    void Update() override;

private:
    void GenerateVertexBuffers();
    
    std::vector<MeshOpenGL*> _mesh;
    Materials::BasicShapeMaterialImpl _material = {"Shape2DMaterial"};
    SceneParameters _sceneParameters;
    std::shared_ptr<PipelineProgram> _shaderProgram;
};
    
} // namespace Snowglobe::RenderOpenGL
