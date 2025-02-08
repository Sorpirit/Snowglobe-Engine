#include "RenderEngineSyncSystem.hpp"

#include "MeshComponent.hpp"
#include "TransformComponent.hpp"

namespace Snowglobe::Engine
{

void RenderEngineSyncSystem::UpdateLate()
{
    auto& entities = _entityManager->GetAllEntities();
    for (auto& entity : entities)
    {
        if (entity->IsActive() == false)
            continue;

        Core::TransformComponent* transform = nullptr;
        MeshComponent* mesh = nullptr;
        if (!entity->QueryComponents(transform, mesh))
            continue;

        mesh->GetMeshProxy()->SetPosition(transform->Position);
        mesh->GetMeshProxy()->SetRotation(transform->Rotation);
        mesh->GetMeshProxy()->SetScale(transform->Scale);
    }
}
    
}


