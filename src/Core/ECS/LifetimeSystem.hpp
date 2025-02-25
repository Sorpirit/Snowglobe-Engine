#pragma once
#include "EntityManager.hpp"
#include "Lifetime.hpp"
#include "SystemManager.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace Snowglobe::Core::ECS
{

class LifetimeSystem
{
public:

    void Init(const std::shared_ptr<EntityManagerBase>& entity_manager,
                   const std::shared_ptr<SystemManager>& system_manager)
    {
        _entityManager = entity_manager;
        _systemManager = system_manager;
    }

    Lifetime Register(std::string debugName);
    Lifetime Register(std::string debugName, Lifetime baseLifetime);
    void Expire(Lifetime lifetime);
private:
    uint32_t _nextID = 1;
    std::unordered_map<Lifetime, std::string> _lifetimes;
    std::unordered_map<Lifetime, std::vector<Lifetime>> _dependencies;

    std::shared_ptr<EntityManagerBase> _entityManager;
    std::shared_ptr<SystemManager> _systemManager;
};

}