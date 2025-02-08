#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "Tag.hpp"
#include "Entity.hpp"

namespace Snowglobe::Core::ECS
{

/// @brief Base class for EntityManagers. Provides the basic interface for creating and managing entities and not dependent on the EntityData type.
class EntityManagerBase
{
public:
    virtual ~EntityManagerBase() = default;

    /// @brief Create a new entity with the given tag.
    /// @return Shared pointer to the created entity. All the entities are stored in the EntityManager.
    virtual std::shared_ptr<Entity> CreateEntity(Tag tag = Tags::Default()) = 0;
    
    /// @brief Get all entities with the given tag.
    /// @return Vector of shared pointers to the entities with the given tag.
    virtual std::vector<std::shared_ptr<Entity>>& GetEntitiesByTag(Tag tag) = 0;

    /// @brief Get all entities that are managed by the EntityManager.
    /// @return Vector of shared pointers to all entities.
    virtual std::vector<std::shared_ptr<Entity>>& GetAllEntities() = 0;

    /// @brief Updates the EntityManager. This will update all the entities, destroy and detach the components from the entities that marked for destruction. Also, it will add the new entities to the EntityManager.
    /// This function should be called once per frame at the start or at the end of the frame.
    virtual void Update() = 0;
};

/// @brief EntityManager class that manages entities with the given EntityData type.
template <class TEntityData>
class EntityManager : public EntityManagerBase
{
public:
    std::shared_ptr<Entity> CreateEntity(Tag tag = Tags::Default()) override
    {
        auto entityData = std::make_shared<TEntityData>();
        EntityData* entityDataPtr = entityData.get();
        auto entity = std::make_shared<Entity>( entityDataPtr, _nextId, tag, "Entity_" + std::to_string(_nextId));
        _toAdd.push_back({entity, entityData});
        _nextId++;
        return entity;
    }

    std::vector<std::shared_ptr<Entity>>& GetEntitiesByTag(Tag tag) override
    {
        return _entityMap[tag];
    }

    std::vector<std::shared_ptr<Entity>>& GetAllEntities() override
    {
        return _entities;
    }

    void Update() override
    {
        // In entity update we detach all components that was removed in the previous frame. 
        // If the entity is destroyed, we detach all components.
        for (auto& entityPair : _entityPairs)
        {
            std::get<std::shared_ptr<Entity>>(entityPair)->Update();
        }

        std::erase_if(_entityPairs, [](auto& entityPair)
        {
            auto entity = std::get<std::shared_ptr<Entity>>(entityPair);
            bool isDestroyed = entity->IsDestroyed();
            // 1(_entityPairs) + 1(_entities) + 1(_entityMap) + 1(entity) = 4
            assert((!isDestroyed || entity.use_count() <= 4) && "Unable to destroy entity because it is referenced somewhere else"); 
            return isDestroyed;
        });
        std::erase_if(_entities, [](auto& entity) { return entity->IsDestroyed(); });
        for (auto& entities : _entityMap | std::views::values)
        {
            std::erase_if(entities, [](auto& entity) { return entity->IsDestroyed(); });
        }

        for (auto& entityPair : _toAdd)
        {
            _entityPairs.push_back(entityPair);
            _entityMap[std::get<std::shared_ptr<Entity>>(entityPair)->GetTag()].push_back(std::get<std::shared_ptr<Entity>>(entityPair));
            _entities.push_back(std::get<std::shared_ptr<Entity>>(entityPair));
        }

        _toAdd.clear();
    }

private:
    typedef std::tuple<std::shared_ptr<Entity>, std::shared_ptr<TEntityData>> EntityPair;
    std::vector<EntityPair> _entityPairs;
    
    std::vector<std::shared_ptr<Entity>> _entities;
    std::unordered_map<Tag, std::vector<std::shared_ptr<Entity>>> _entityMap;

    size_t _nextId = 0;
    std::vector<EntityPair> _toAdd;

};

}