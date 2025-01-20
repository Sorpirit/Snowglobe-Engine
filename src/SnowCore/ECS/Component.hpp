#pragma once

namespace Snowglobe::SnowCore::ECS
{

/// @brief Base class for all components. Naming convention is NameComponent. 
/// Component should contain onle data and no logic(appart from described here: OnAttach, OnDetach, OnActivate, OnDeactivate).
class Component
{
public:
    /// @brief Defines if component is attached to an entity.
    /// @return True if component is attached to an entity.
    bool IsAttached() const { return _attached; }

    /// @brief Defines if component is active.
    /// @return True if component is active.
    bool IsActive() const { return _isActive; }

    virtual void OnAttach() { _attached = true; }
    virtual void OnDetach() { _attached = false; }

    virtual void OnActivate() { _isActive = true; }
    virtual void OnDeactivate() { _isActive = false; }

    void OnRemove() { _needsToBeRemoved = true; }
    bool NeedsToBeDetached() const { return _needsToBeRemoved; }
    
private:
    bool _attached = false;
    bool _isActive = true;
    bool _needsToBeRemoved = false;
};
    
} // namespace Snowglobe::SnowCore::ESC


