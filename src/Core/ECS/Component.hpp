#pragma once

namespace Snowglobe::Core::ECS
{

/// @brief Base class for all components. Naming convention is NameComponent. 
/// Component should contain only data and no logic(apart from described here: OnAttach, OnDetach, OnActivate, OnDeactivate).
class Component
{
public:
    virtual ~Component() = default;

    /// @brief Defines if component is attached to an entity. Only entity should be able to change this state.
    /// @return True if component is attached to an entity.
    bool IsAttached() const { return _attached; }

    /// @brief Defines if component is active. Only entity should be able to change this state.
    /// @return True if component is active.
    bool IsActive() const { return _isActive; }

    /// @brief Called when component is attached to an entity. Called from Entity::AddComponent.
    virtual void OnAttach() { _attached = true; }
    /// @brief Called when component is detached from an entity. Called from Entity::RemoveComponent or when entity is destroyed.
    virtual void OnDetach() { _attached = false; }

    /// @brief Called when component is activated. Called on attached after OnAttach if entity is Active. Every time entity is activated OnActivate will be called.
    virtual void OnActivate() { _isActive = true; }
    /// @brief Called when component is deactivated. Every time entity is deactivated OnDeactivate will be called. 
    virtual void OnDeactivate() { _isActive = false; }

    /// @brief Activates delayed removal process. Component will be removed only on the next EntityManager Update. Should be called only from the Entity.
    void OnRemove() { _needsToBeRemoved = true; }
    /// @brief Used for delayed removal process. Should be called only from the Entity.
    bool NeedsToBeDetached() const { return _needsToBeRemoved; }
    
private:
    bool _attached = false;
    bool _isActive = true;
    bool _needsToBeRemoved = false;
};
    
} // namespace Snowglobe::Core::ESC


