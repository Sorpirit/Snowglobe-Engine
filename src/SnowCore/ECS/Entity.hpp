#pragma once

#include <functional>
#include <vector>
#include <type_traits>

#include "Component.hpp"
#include "EntityData.hpp"

namespace Snowglobe::SnowCore::ECS
{

class Entity
{
public:
    Entity(EntityData& data, uint32_t id, uint32_t tag, const std::string& name) : _tag(tag), _id(id), _components(data), _name(name) {}

    template <class TComponent, typename... TArgs>
    bool AddComponent(TArgs&&... args)
    {
        static_assert(std::is_base_of<Component, TComponent>::value, "TComponent must incherit from Component");
        auto& component = _components.GetComponent<TComponent>();

        if(component.IsAttached())
            return false;
        
        component = TComponent(std::forward<TArgs>(args)...);
        component.OnAttach();

        if(_isActive)
            component.OnActivate();
        
        return true;
    }

    template <class TComponent>
    bool RemoveComponent()
    {
        static_assert(std::is_base_of<Component, TComponent>::value, "TComponent must incherit from Component");
        auto& component = _components.GetComponent<TComponent>();

        if(!component.IsAttached())
            return false;
        
        component.OnRemove();
        return true;
    }

    template <class TComponent>
    bool HasComponent() const
    {
        static_assert(std::is_base_of<Component, TComponent>::value, "TComponent must incherit from Component");
        auto& component = _components.GetComponent<TComponent>();
        return component.IsAttached();
    }

    template <class TComponent>
    bool QueryComponent(TComponent*& componentPtr)
    {
        static_assert(std::is_base_of<Component, TComponent>::value, "TComponent must incherit from Component");

        componentPtr = nullptr;
        auto& component = _components.GetComponent<TComponent>();
        if(!component.IsAttached())
            return false;
        
        componentPtr = &component;
        return true;
    }

    template <class... TQComponents>
    bool QueryComponents(TQComponents*&... componentPtrs)
    {
        return (QueryComponent(componentPtrs) && ...);
    }

    void ListAttachedComponents(std::vector<Component*>& components)
    {
        _components.ForEachComponent([&components](Component& component)
        {
            if (!component.IsAttached())
                return;
            
            components.push_back(&component);
        });
    }

    void SetActive(bool isActive)
    {
        if(_isActive == isActive)
            return;
            
        _isActive = isActive;
        _components.ForEachComponent([isActive](Component& component)
        {
            if (!component.IsAttached())
                return;

            if(isActive)
                component.OnActivate();
            else
                component.OnDeactivate();
        });
    }
    bool IsActive() const { return _isActive; }

    void Destroy()
    {
        if(_isDestroyed)
            return;

        _isDestroyed = true;
        _components.ForEachComponent([](Component& component)
        {
            if (!component.IsAttached())
                return;

            component.OnRemove();
        });
    }

    bool IsDestroyed() const { return _isDestroyed; }
    uint32_t GetTag() const { return _tag; }

    bool operator==(const Entity& other) const { return _id == other._id; }

    void Update() const
    {
        _components.ForEachComponent([](Component& component)
        {
            if (!component.IsAttached())
                return;

            if(component.NeedsToBeDetached())
            {
                component.OnDetach();
            }
        });
    }

    void SetName(const std::string& name) { _name = name; }
    const std::string& GetName() const { return _name; }

private:
    bool _isActive = true;
    bool _isDestroyed = false;
    uint32_t _tag = 0;
    uint32_t _id = 0;

    std::string _name = "Entity";
    
    EntityData& _components;
};
    
} // namespace Snowglobe::SnowCore::ESC
