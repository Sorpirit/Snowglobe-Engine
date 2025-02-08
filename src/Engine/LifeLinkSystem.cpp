#include "LifeLinkSystem.hpp"

#include "LifeLinkComponent.hpp"

namespace Snowglobe::Engine
{

void LifeLinkSystem::UpdateLate()
{
    for (const auto& entity : _entityManager->GetAllEntities())
    {
        Core::LifeLinkComponent* lifeLinkComponent;
        if (entity->QueryComponent(lifeLinkComponent))
        {
            bool isDestroyed = entity->IsDestroyed();
            if (!isDestroyed)
                continue;
                
            for (const auto& linkedEntity : lifeLinkComponent->LinkedEntities)
            {
                if (auto sharedEntity = linkedEntity.lock())
                {
                    sharedEntity->Destroy();
                }
            }
        }
    }
}
    
}


