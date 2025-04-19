#pragma once

#include "ISystem.hpp"
#include "UpdateOrder.hpp"

#include <map>

namespace Snowglobe::Core::ECS
{

class SystemManager
{
public:
    explicit SystemManager(const std::shared_ptr<EntityManagerBase>& entityManager) : _entityManager(entityManager) {}

    template <class T, typename... TArgs> bool TryAddSystem(Lifetime lifetime = DefaultLifetime, TArgs&&... args)
    {
        static_assert(std::is_base_of_v<ISystem, T>, "T must derive from ISystem");
        auto entry = _typedSystemsMap.find(typeid(T));
        if (entry != _typedSystemsMap.end())
        {
            return false;
        }

        auto system = std::make_shared<T>(std::forward<TArgs>(args)...);
        system->Init(_entityManager);
        _typedSystemsMap[typeid(T)] = system;
        _updateOrderSystemsMap[system->GetUpdateOrder()].push_back(system);
        _lifetimeSystemMap[lifetime].push_back(system);
        return true;
    }

    bool TryAddSystem(FAnonymousUpdate update, uint32_t order = UpdateOrder::PrePhysics, Lifetime lifetime = DefaultLifetime, const std::string& debugName="AnonymousSystem")
    {
        auto system = std::make_shared<AnonymousSystem>(debugName, update);
        system->Init(_entityManager);
        _updateOrderSystemsMap[order].push_back(system);
        _lifetimeSystemMap[lifetime].push_back(system);
        return true;
    }

    template <class T> bool TryAddSystem(std::shared_ptr<ISystem> system, Lifetime lifetime = DefaultLifetime)
    {
        static_assert(std::is_base_of_v<ISystem, T>, "T must derive from ISystem");
        auto entry = _typedSystemsMap.find(typeid(T));
        if (entry != _typedSystemsMap.end())
        {
            return false;
        }

        system->Init(_entityManager);
        _typedSystemsMap[typeid(T)] = system;
        _updateOrderSystemsMap[system->GetUpdateOrder()].push_back(system);
        _lifetimeSystemMap[lifetime].push_back(system);
        return true;
    }

    template <typename T> bool QuerySystem(T*& ptr)
    {
        static_assert(std::is_base_of_v<ISystem, T>, "T must derive from ISystem");

        auto entry = _typedSystemsMap.find(typeid(T));

        if (entry != _typedSystemsMap.end())
        {
            ptr = dynamic_cast<T*>(entry->second.get());
            return true;
        }

        return false;
    }

    std::unordered_map<std::type_index, std::shared_ptr<ISystem>>& GetTypedSystems() { return _typedSystemsMap;  }
    auto GetSortedSystems() { return _updateOrderSystemsMap | std::views::values;  }
    std::vector<std::shared_ptr<ISystem>>& GetSystemsWithLifetime(Lifetime lifetime) { return _lifetimeSystemMap[lifetime]; }

    void Update()
    {
        for (auto systems : _updateOrderSystemsMap | std::views::values)
        {
            for (auto system : systems)
            {
                if (!system->IsActive())
                    continue;

                system->Update();
            }
        }

        for (auto& systems : _updateOrderSystemsMap | std::views::values)
        {
            std::erase_if(systems, [](auto& system) { return system->NeedsUnregisterSystem(); });
        }

        for (auto& systems : _lifetimeSystemMap | std::views::values)
        {
            std::erase_if(systems, [](auto& system) { return system->NeedsUnregisterSystem(); });
        }

        std::erase_if(_typedSystemsMap, [](auto& systemPair) {
            auto const& [key, system] = systemPair;
            assert((!system->NeedsUnregisterSystem() || system.use_count() <= 1) &&
                   "Unable to destroy system because it is referenced somewhere else");

            return system->NeedsUnregisterSystem();
        });
    }

  private:
    std::shared_ptr<EntityManagerBase> _entityManager;
    std::unordered_map<std::type_index, std::shared_ptr<ISystem>> _typedSystemsMap;
    std::map<uint32_t, std::vector<std::shared_ptr<ISystem>>> _updateOrderSystemsMap;
    std::unordered_map<Lifetime, std::vector<std::shared_ptr<ISystem>>> _lifetimeSystemMap;
};

} // namespace Snowglobe::Core::ECS