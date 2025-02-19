#pragma once
#include "ECS/EntityManager.hpp"
#include "ECS/ISystem.hpp"
#include "Materials/BasicShapeMaterialImpl.hpp"
#include "Materials/TextureShapeMaterialImpl.hpp"
#include "MeshOpenGL.hpp"
#include "SceneParameters.hpp"

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
    Materials::TextureShapeMaterialImpl _material = {"SpriteMaterial"};
    SceneParameters _sceneParameters;
    uint32_t _shaderProgram;
};

} // namespace Snowglobe::RenderOpenGL