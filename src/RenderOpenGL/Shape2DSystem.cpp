#include "Shape2DSystem.hpp"

#include <numbers>

#include "CommonVertexLayouts.hpp"
#include "NEdgeShape2DComponent.hpp"

#include "OpenGLRenderSystem.hpp"
#include "TransformComponent.hpp"

namespace Snowglobe::RenderOpenGL
{
    void Shape2DSystem::Init(std::shared_ptr<Core::ECS::EntityManagerBase> entityManager)
    {
        ISystem::Init(entityManager);
        
        auto shaderCompiler = OpenGLRenderSystem::GetInstance()->GetShaderCompiler();

        GraphicsPipelineCreateInfo params;
        params.VertexShader = Core::SnowFileHandle("Shape2D.vert");
        params.FragmentShader = Core::SnowFileHandle("Shape2D.frag");
        _shaderProgram = shaderCompiler->GetOrCratePipeline(params);
        GenerateVertexBuffers();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Shape2DSystem::Update()
    {
        _shaderProgram->Bind();

        _sceneParameters.Bind(OpenGLRenderSystem::GetInstance()->GetCamera(), _shaderProgram->GetProgramID());

        auto& entities = _entityManager->GetAllEntities();
        for (auto& entity : entities)
        {
            Core::TransformComponent* transform = nullptr;
            Render::NEdgeShape2DComponent* shape2DComponent = nullptr;
            if (!entity->QueryComponents(transform, shape2DComponent))
            {
                continue;
            }
            
            auto mesh = _mesh[shape2DComponent->EdgeCount - 3];
            mesh->SetPosition(transform->Position);
            mesh->SetRotation(transform->Rotation);
            mesh->SetScale(transform->Scale);

            if (shape2DComponent->UseOutline)
            {
                mesh->SetPosition(transform->Position - glm::vec3(0, 0, 0.1f));
                mesh->Bind(_shaderProgram->GetProgramID());
                _material.GetMaterialData().color = glm::vec4(shape2DComponent->Outline, shape2DComponent->Alpha);
                _material.Bind(_shaderProgram->GetProgramID());
                mesh->Draw();

                mesh->SetPosition(transform->Position);
                mesh->SetScale(transform->Scale - glm::vec3(shape2DComponent->OutlineWidth));
            }
            
            mesh->Bind(_shaderProgram->GetProgramID());
            _material.GetMaterialData().color = glm::vec4(shape2DComponent->Color, shape2DComponent->Alpha);
            _material.Bind(_shaderProgram->GetProgramID());
            mesh->Draw();
        }
    }

    void Shape2DSystem::GenerateVertexBuffers()
    {
        auto renderer = OpenGLRenderSystem::GetInstance();
        
        std::vector<Render::PositionVertex> vertices;
        float scale = 0.5f;

        for (int i = 3; i < 33; ++i)
        {
            float angle = (2.0f * std::numbers::pi_v<float>) / i;
            for (int j = 1; j < i; ++j)
            {
                vertices.push_back({{0, 0, 0}});
                vertices.push_back({scale * glm::vec3{cos(angle * j), sin(angle * j), 0}});
                vertices.push_back({scale * glm::vec3{cos(angle * (j - 1)), sin(angle * (j - 1)), 0}});
            }

            vertices.push_back({{0, 0, 0}});
            vertices.push_back({scale * glm::vec3{cos(0.0f), sin(0.0f), 0}});
            vertices.push_back({scale * glm::vec3{cos(angle * (i - 1)), sin(angle * (i - 1)), 0}});
            
            auto vertexBuffer = renderer->AllocateVertexBufferPtr(vertices, "VertexEdgeShape2D_" + std::to_string(i));
            auto mesh = renderer->CreateMeshProxy(*vertexBuffer, "EdgeShape2D_" + std::to_string(i));
            _mesh.push_back(static_cast<MeshOpenGL*>(mesh));
            vertices.clear();
        }
    }
} // namespace Snowglobe::RenderOpenGL
