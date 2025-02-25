#include "LifetimeSystem.hpp"

namespace Snowglobe::Core::ECS
{

Lifetime LifetimeSystem::Register(std::string debugName)
{
    Lifetime result(_nextID++);
    _lifetimes[result] = debugName;
    _dependencies[DefaultLifetime].push_back(result);
    return result;
}

Lifetime LifetimeSystem::Register(std::string debugName, Lifetime baseLifetime)
{
    Lifetime result(_nextID++);
    _lifetimes[result] = debugName;
    _dependencies[baseLifetime].push_back(result);
    return result;
}

void LifetimeSystem::Expire(Lifetime lifetime)
{
    for (auto dependency : _dependencies[lifetime])
    {
        Expire(dependency);
    }
    _dependencies.erase(lifetime);

    for (auto& entity : _entityManager->GetEntitiesWithLifetime(lifetime))
    {
        entity->Destroy();
    }

    for (auto& system : _systemManager->GetSystemsWithLifetime(lifetime))
    {
        system->UnregisterSystem();
    }
}

} // namespace Snowglobe::Core::ECS