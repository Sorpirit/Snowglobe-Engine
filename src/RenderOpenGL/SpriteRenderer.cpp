#include "SpriteRenderer.hpp"

#include "CommonVertexLayouts.hpp"
#include "OpenGLRenderSystem.hpp"
#include "ShaderCompiler.hpp"
#include "SpriteRenderComponent.hpp"
#include "TransformComponent.hpp"

namespace Snowglobe::RenderOpenGL
{

void SpriteRenderer::Init(std::shared_ptr<Core::ECS::EntityManagerBase> entityManager)
{
    ISystem::Init(entityManager);

    auto shaderCompiler = OpenGLRenderSystem::GetInstance()->GetShaderCompiler();
    auto vertexShader = shaderCompiler->GetOrCompileShader(Core::SnowFileHandle("sprite.vert"));
    auto fragmentShader = shaderCompiler->GetOrCompileShader(Core::SnowFileHandle("sprite.frag"));

    PipelineSetupParams params = {vertexShader, fragmentShader};
    _shaderProgram = shaderCompiler->GetOrCratePipeline(params);

    GenerateVertexBuffers();
}

void SpriteRenderer::Update()
{
    glUseProgram(_shaderProgram);
    _sceneParameters.Bind(OpenGLRenderSystem::GetInstance()->GetCamera(), _shaderProgram);
    _quadMesh->Bind(_shaderProgram);

    auto& entities = _entityManager->GetAllEntities();
    for (auto& entity : entities)
    {
        Core::TransformComponent* transform = nullptr;
        Render::SpriteRenderComponent* sprite = nullptr;
        if (!entity->QueryComponents(transform, sprite))
            continue;

        _quadMesh->SetPosition(transform->Position);
        _quadMesh->SetRotation(transform->Rotation);
        _quadMesh->SetScale(transform->Scale);
        _quadMesh->BindEntity(_shaderProgram);

        auto& spriteData = sprite->SpriteAsset.GetData();
        _material.GetMaterialData().texture = sprite->SpriteAsset.HasData() ? spriteData.GetTexture() : Render::Texture2DPtr{};
        _material.GetMaterialData().color = glm::vec4(sprite->Color, 1);
        _material.GetMaterialData().textureOffset = spriteData.CurrentSpriteCoord;
        _material.GetMaterialData().textureScale = spriteData.SpriteScale;
        _material.Bind(_shaderProgram);

        _quadMesh->Draw();
    }
}

void SpriteRenderer::GenerateVertexBuffers()
{
    auto renderer = OpenGLRenderSystem::GetInstance();

    std::vector<Render::PositionUVVertex> vertices;
    vertices.push_back({{-0.5f, -0.5f, 0.0f}, {0, 1}});
    vertices.push_back({{-0.5f, 0.5f, 0.0f}, {0, 0}});
    vertices.push_back({{0.5f, 0.5f, 0.0f}, {1, 0}});
    vertices.push_back({{-0.5f, -0.5f, 0.0f}, {0, 1}});
    vertices.push_back({{0.5f, 0.5f, 0.0f}, {1, 0}});
    vertices.push_back({{0.5f, -0.5f, 0.0f}, {1, 1}});


    auto vertexBuffer = renderer->AllocateVertexBufferPtr(vertices, "QuadVB");
    _quadMesh = static_cast<MeshOpenGL*>(renderer->CreateMeshProxy(*vertexBuffer, "QuadMesh"));
}

} // namespace Snowglobe::RenderOpenGL