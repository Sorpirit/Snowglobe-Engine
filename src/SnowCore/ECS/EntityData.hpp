#pragma once

#include <functional>
#include <any>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <type_traits>

#include "Component.hpp"

namespace Snowglobe::SnowCore::ECS
{


class EntityData
{
public:
    template <class TComponent>
    TComponent& GetComponent()
    {
        static_assert(std::is_base_of<Component, TComponent>::value, "TComponent must incherit from Component");
        return static_cast<TComponent&>(GetComponent(std::type_index(typeid(TComponent))));
    }

    virtual void ForEachComponent(const std::function<void(Component&)>& func) = 0;
private:
    virtual Component& GetComponent(std::type_index type) = 0;
    
};
    
template <class... TComponents>
class MapEntityData : public EntityData
{
    
public:
    MapEntityData()
    {
        InitializeComponentMap();
    }
    
    Component& GetComponent(std::type_index type) override
    {
        auto it = _componentsMap.find(type);
        assert(it != _componentsMap.end() && "Entity trying to get component that does not exist in EntityData");
        return *it->second;
    }
    
    void ForEachComponent(const std::function<void(Component&)>& func) override
    {
        for (auto& [type, component] : _componentsMap)
        {
            func(*component);
        }
    }
    
private:
    // std::unordered_map<std::type_index, std::any> _components;
    
    std::unordered_map<std::type_index, Component*> _componentsMap;
    std::tuple<TComponents...> _componentsTuple;

    void InitializeComponentMap() {
        // Iterate over the tuple and populate the map
        std::apply([this]<typename... T0>(T0&... component) {
            ((_componentsMap[std::type_index(typeid(std::decay_t<T0>))] = &component), ...);
        }, _componentsTuple);
    }
};

}