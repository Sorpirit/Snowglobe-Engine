#include "RenderEngineSyncSystem.hpp"

#include "MeshComponent.hpp"
#include "TransformComponent.hpp"

namespace Snowglobe::Engine
{

void RenderEngineSyncSystem::Update()
{
    auto& entities = _entityManager->GetAllEntities();
    for (auto& entity : entities)
    {
        if (entity->IsActive() == false)
            continue;

        Core::TransformComponent* transform = nullptr;
        if (!entity->QueryComponent(transform))
            continue;

        MeshComponent* mesh = nullptr;
        ModelComponent* model = nullptr;
        if (entity->QueryComponent(mesh))
        {
            mesh->GetMeshProxy()->SetPosition(transform->Position);
            mesh->GetMeshProxy()->SetRotation(transform->Rotation);
            mesh->GetMeshProxy()->SetScale(transform->Scale);
        }
        else if (entity->QueryComponent(model))
        {
            for (auto mesh : model->GetMeshProxyLists())
            {
                mesh->SetPosition(transform->Position);
                mesh->SetRotation(transform->Rotation);
                mesh->SetScale(transform->Scale);
            }
        }
    }
}
    
}


