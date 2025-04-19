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

    GraphicsPipelineCreateInfo params;
    params.VertexShader = Core::SnowFileHandle("sprite.vert");
    params.FragmentShader = Core::SnowFileHandle("sprite.frag");
    _shaderProgram = shaderCompiler->GetOrCratePipeline(params);

    GenerateVertexBuffers();
}

void SpriteRenderer::Update()
{
    _shaderProgram->Bind();
    _sceneParameters.Bind(OpenGLRenderSystem::GetInstance()->GetCamera(), _shaderProgram->GetProgramID());
    _quadMesh->Bind(_shaderProgram->GetProgramID());

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
        _quadMesh->BindEntity(_shaderProgram->GetProgramID());

        _material.GetMaterialData().texture = sprite->SpriteAsset.HasData() ? sprite->SpriteAsset.GetData().GetTexture() : Render::Texture2DPtr{};
        _material.GetMaterialData().color = glm::vec4(sprite->Color, 1);
        _material.GetMaterialData().flipXY = (Render::MaterialsData::FLIP_X && sprite->flipX ) || (Render::MaterialsData::FLIP_Y && sprite->flipY);

        if (sprite->SpriteAsset.HasData())
        {
            auto& spriteData = sprite->SpriteAsset.GetData();

            _material.GetMaterialData().textureOffset = sprite->SpriteAsset.GetData().CurrentSpriteCoord;
            _material.GetMaterialData().textureScale = sprite->SpriteAsset.GetData().SpriteScale;
        }
        else
        {
            _material.GetMaterialData().textureOffset = glm::vec2(0);
            _material.GetMaterialData().textureScale = glm::vec2(1);
        }
        
        _material.Bind(_shaderProgram->GetProgramID());
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