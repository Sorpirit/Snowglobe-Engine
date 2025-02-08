#include "LifetimeSystem.hpp"

#include "EngineTime.hpp"
#include "LifetimeComponent.hpp"

namespace Snowglobe::Engine
{

void LifetimeSystem::Update()
{
    auto& entities = _entityManager->GetAllEntities();
    float deltaTime = Core::EngineTime::GetDeltaEngineFrameTime();
    for (auto& entity : entities)
    {
        if (entity->IsActive() == false)
            continue;

        LifetimeComponent* lifetime = nullptr;
        if (!entity->QueryComponent(lifetime))
            continue;

    
        lifetime->Lifetime -= deltaTime;
        if (lifetime->Lifetime <= 0.0f)
        {
            entity->Destroy();
        }
    }
}
    
}


