#pragma once

#include <any>
#include <cassert>
#include <functional>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

#include "Component.hpp"

namespace Snowglobe::Core::ECS
{

/// @brief Base class for all entity data. Provides an abstraction for how components are stored. All the entities
/// should use the same EntityData type.
class EntityData
{
  public:
    virtual ~EntityData() = default;

    /// @brief Get a reference to the component of type TComponent from the entity data. TComponent must inherit from
    /// Component.
    /// @return Reference to the component of type TComponent.
    template <class TComponent> TComponent& GetComponent()
    {
        static_assert(std::is_base_of_v<Component, TComponent>, "TComponent must inherit from Component");
        return static_cast<TComponent&>(GetComponent(std::type_index(typeid(TComponent))));
    }

    /// @brief Iterate over all components in the entity data and call the provided function with each component.
    virtual void ForEachComponent(const std::function<void(Component&)>& func) = 0;

    virtual void CopyFrom(const EntityData& source) = 0;
  private:
    virtual Component& GetComponent(std::type_index type) = 0;
};

/// @brief Entity data that stores components in a continuous tuple and provides runtime access to them via
/// unordered_map which stores pointers to the components in the tuple.
template <class... TComponents> class MappedTupleEntityData : public EntityData
{
  public:
    MappedTupleEntityData() { InitializeComponentMap(); }

    Component& GetComponent(std::type_index type) override
    {
        auto it = _componentsMap.find(type);
        assert(it != _componentsMap.end() && "Entity trying to get component that does not exist in EntityData");
        return *it->second;
    }

    void ForEachComponent(const std::function<void(Component&)>& func) override
    {
        for (auto& component : _componentsMap | std::views::values)
        {
            func(*component);
        }
    }

    void CopyFrom(const EntityData& source) override
    {
        _componentsMap.clear();
        const MappedTupleEntityData& typedSource = static_cast<const MappedTupleEntityData&>(source);
        _componentsTuple = typedSource._componentsTuple;
        InitializeComponentMap();
    }

  private:
    std::tuple<TComponents...> _componentsTuple;
    std::unordered_map<std::type_index, Component*> _componentsMap;

    void InitializeComponentMap()
    {
        // Iterate over the tuple and populate the map
        std::apply(
            [this]<typename... T0>(T0&... component) {
                ((_componentsMap[std::type_index(typeid(std::decay_t<T0>))] = &component), ...);
            },
            _componentsTuple);
    }
};

} // namespace Snowglobe::Core::ECS