#pragma once
#include "ECS/EntityManager.hpp"
#include "ECS/ISystem.hpp"
#include "Materials/BasicShapeMaterialImpl.hpp"
#include "Materials/SpriteMaterial.hpp"
#include "MeshOpenGL.hpp"
#include "SceneParameters.hpp"
#include "ShaderCompiler.hpp"

#include <complex.h>
#include <memory>

namespace Snowglobe::RenderOpenGL
{

class SpriteRenderer : public Core::ECS::ISystem
{
public:
    void Init(std::shared_ptr<Core::ECS::EntityManagerBase> entityManager) override;

    void Update() override;
private:
    void GenerateVertexBuffers();

    MeshOpenGL* _quadMesh;
    Materials::SpriteMaterial _material = {"SpriteMaterial"};
    SceneParameters _sceneParameters;
    std::shared_ptr<PipelineProgram> _shaderProgram;
};

} // namespace Snowglobe::RenderOpenGL