#include "RenderEngineSyncSystem.hpp"

#include "TransformComponent.hpp"

namespace Snowglobe::SnowEngine
{

void RenderEngineSyncSystem::UpdateLate()
{
    auto& entities = _entityManager.GetAllEntities();
    for (auto& entity : entities)
    {
        if (entity->IsActive() == false)
            continue;

        SnowCore::TransformComponent* transform = nullptr;
        MeshComponent* mesh = nullptr;
        if (!entity->QueryComponents(transform, mesh))
            continue;

        mesh->GetMeshProxy()->SetPosition(transform->Position);
        mesh->GetMeshProxy()->SetRotation(transform->Rotation);
        mesh->GetMeshProxy()->SetScale(transform->Scale);
    }
}

}


