#pragma once

#include <functional>
#include <type_traits>
#include <vector>

#include "Tag.hpp"
#include "Component.hpp"
#include "EntityData.hpp"

namespace Snowglobe::Core::ECS
{

/// @brief Stores and manages components of an entity
class Entity
{
public:
    Entity(EntityData* data, uint32_t id, Tag tag, std::string name) : _tag(tag), _id(id), _name(std::move(name)), _components(data) {}

    bool operator==(const Entity& other) const { return _id == other._id; }
    
    /// @brief Adds a component to the entity. Component mast inherit from Component, and must be defined in the EntityData template
    /// @return True if component is added, false if component is already attached
    template <class TComponent, typename... TArgs>
    bool AddComponent(TArgs&&... args)
    {
        static_assert(std::is_base_of_v<Component, TComponent>, "TComponent must inherit from Component");
        auto& component = _components->GetComponent<TComponent>();

        if(component.IsAttached())
            return false;
        
        component = TComponent(std::forward<TArgs>(args)...);
        component.OnAttach();

        if(_isActive)
            component.OnActivate();
        
        return true;
    }

    /// @brief Removes a component from the entity
    /// @return True if component is removed, false if component is not attached
    template <class TComponent>
    bool RemoveComponent()
    {
        static_assert(std::is_base_of_v<Component, TComponent>, "TComponent must inherit from Component");
        auto& component = _components->GetComponent<TComponent>();

        if(!component.IsAttached())
            return false;
        
        component.OnRemove();
        return true;
    }

    /// @brief Checks if a component is attached to the entity
    /// @return True if component is attached, false if component is not attached
    template <class TComponent>
    bool HasComponent() const
    {
        static_assert(std::is_base_of_v<Component, TComponent>, "TComponent must inherit from Component");
        auto& component = _components->GetComponent<TComponent>();
        return component.IsAttached();
    }

    /// @brief Queries a component from the entity. If component is attached, componentPtr is set to the component
    /// @return True if component is attached, false if component is not attached
    template <class TComponent>
    bool QueryComponent(TComponent*& componentPtr)
    {
        static_assert(std::is_base_of_v<Component, TComponent>, "TComponent must inherit from Component");

        componentPtr = nullptr;
        auto& component = _components->GetComponent<TComponent>();
        if(!component.IsAttached())
            return false;
        
        componentPtr = &component;
        return true;
    }

    /// @brief Queries multiple components from the entity. If all components are attached, componentPointers are set to the components
    /// @return True if ALL components are attached, false if ANY component is not attached
    template <class... TComponents>
    bool QueryComponents(TComponents*&... componentPointers)
    {
        return (QueryComponent(componentPointers) && ...);
    }

    /// @brief Lists all attached components of the entity. Components are added to the components vector
    void ListAttachedComponents(std::vector<Component*>& components) const
    {
        _components->ForEachComponent([&components](Component& component)
        {
            if (!component.IsAttached())
                return;
            
            components.push_back(&component);
        });
    }

    /// @brief Sets the entity to active or inactive. If entity is set to active, all attached components are activated as well and vice versa
    void SetActive(bool isActive)
    {
        if(_isActive == isActive)
            return;
            
        _isActive = isActive;
        _components->ForEachComponent([isActive](Component& component)
        {
            if (!component.IsAttached())
                return;

            if(isActive)
                component.OnActivate();
            else
                component.OnDeactivate();
        });
    }

    /// @brief Checks if the entity is active
    /// @return True if entity is active, false if entity is inactive
    bool IsActive() const { return _isActive; }

    /// @brief Destroys the entity. All attached components are removed.
    /// IMPORTANT: Entity is not destroyed immediately, but is marked for destruction. Entity is destroyed on EntityManager::Update() (at the start of the frame)
    void Destroy()
    {
        if(_isDestroyed)
            return;

        _isDestroyed = true;
        _components->ForEachComponent([](Component& component)
        {
            if (!component.IsAttached())
                return;

            component.OnRemove();
        });
    }

    /// @brief Checks if the entity is marked for destruction
    /// @return True if entity is marked for destruction, false if entity is not marked for destruction
    bool IsDestroyed() const { return _isDestroyed; }

    /// @brief Gets the entity's tag
    /// @return The entity's tag
    Tag GetTag() const { return _tag; }

    /// @brief Updates attached components. If a component is marked for detachment, the component is detached
    void Update() const
    {
        bool isDestroyed = _isDestroyed;
        _components->ForEachComponent([isDestroyed](Component& component)
        {
            if (!component.IsAttached())
                return;

            if(component.NeedsToBeDetached() || isDestroyed)
            {
                component.OnDetach();
            }
        });
    }

    /// @brief Sets entity's debug name
    void SetName(const std::string& name) { _name = name; }
    /// @brief Gets entity's debug name
    const std::string& GetName() const { return _name; }

private:
    bool _isActive = true;
    bool _isDestroyed = false;
    Tag _tag = Tags::Default();
    uint32_t _id = 0;

    std::string _name = "Entity";
    
    EntityData* _components = nullptr;
};
    
}
