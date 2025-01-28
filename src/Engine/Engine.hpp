#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <typeinfo>
#include <functional>

#include "EngineProfile.hpp"
#include "Window.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/ISystem.hpp"

namespace Snowglobe::Engine
{
    
class Engine
{
public:
    static Engine& GetInstance()
    {
        static Engine instance;
        return instance;
    }
    
    ~Engine();

    void Setup(
        const Core::EngineProfile& profile,
        const Render::WindowParams& windowParams,
        std::shared_ptr<Core::ECS::EntityManagerBase> entityManager);
    void Run();

    template <class T, typename... TArgs>
    bool TryAddSystem(TArgs&&... args)
    {
        static_assert(std::is_base_of_v<Core::ECS::ISystem, T>, "T must derive from ISystem");
        auto entry = _systems.find(typeid(T));
        if(entry != _systems.end())
        {
            return false;
        }

        _systems[typeid(T)] = std::make_shared<T>(_entityManager, std::forward<TArgs>(args)...);
        
        return true;
    }
    
    template <typename T>
    bool QuerySystem(T*& ptr)
    {
        static_assert(std::is_base_of_v<Core::ECS::ISystem, T>, "T must derive from ISystem");
        
        auto entry = _systems.find(typeid(T));
        
        if(entry != _systems.end())
        {
            ptr = dynamic_cast<T*>(entry->second.get());
            return true;
        }

        return false;
    }

    void RegisterUpdateCallback(const std::function<void()>& callback) { _updateCallbacks.push_back(callback); }

    std::shared_ptr<Core::ECS::EntityManagerBase> GetEntityManager() const { return _entityManager; }

private:
    Engine() = default;

    std::shared_ptr<Core::ECS::EntityManagerBase> _entityManager = nullptr;
    std::unordered_map<std::type_index, std::shared_ptr<Core::ECS::ISystem>> _systems;
    
    std::vector<std::function<void()>> _updateCallbacks;

    std::string _applicationName;
};
    
}

