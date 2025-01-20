#pragma once

#include <iostream>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <memory>

#include "Entity.hpp"

namespace Snowglobe::SnowCore::ECS
{

class EntityManagerBase
{
public:
    virtual std::shared_ptr<Entity> CreateEntity(uint32_t tag = 0) = 0;
    virtual std::vector<std::shared_ptr<Entity>>& GetEntitiesByTag(uint32_t tag) = 0;
    virtual std::vector<std::shared_ptr<Entity>>& GetAllEntities() = 0;
    virtual void Update() = 0;
};


template <class TEntityData>
class EntityManager : public EntityManagerBase
{
typedef std::tuple<std::shared_ptr<Entity>, std::shared_ptr<TEntityData>> EntityPair;
    
public:
    EntityManager()
    {
        std::cout << "EntityManager created" << std::endl;
    }

    ~EntityManager()
    {
        std::cout << "EntityManager destroyed" << std::endl;
    }

    std::shared_ptr<Entity> CreateEntity(uint32_t tag = 0) override
    {
        auto entityData = std::make_shared<TEntityData>();
        EntityData* entityDataPtr = entityData.get();
        auto entity = std::make_shared<Entity>( *entityDataPtr, _nextId++, tag);
        _toAdd.push_back({entity, entityData});
        return entity;
    }

    std::vector<std::shared_ptr<Entity>>& GetEntitiesByTag(uint32_t tag) override
    {
        return _entityMap[tag];
    }

    std::vector<std::shared_ptr<Entity>>& GetAllEntities() override
    {
        return _entities;
    }

    void Update() override
    {
        // In enitity update we detach all components that was removed in the previous frame. 
        // If the entity is destroyed, we detach all components.
        for (auto& entityPair : _entityPairs)
        {
            std::get<std::shared_ptr<Entity>>(entityPair)->Update();
        }

        std::erase_if(_entityPairs, [](auto& entityPair) { return std::get<std::shared_ptr<Entity>>(entityPair)->IsDestroyed(); });
        std::erase_if(_entities, [](auto& entity) { return entity->IsDestroyed(); });
        for (auto& [tag, entites] : _entityMap)
        {
            std::erase_if(entites, [](auto& entity) { return entity->IsDestroyed(); });
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
    std::vector<EntityPair> _entityPairs;
    
    std::vector<std::shared_ptr<Entity>> _entities;
    std::unordered_map<uint32_t, std::vector<std::shared_ptr<Entity>>> _entityMap;

    size_t _nextId = 0;
    std::vector<EntityPair> _toAdd;

};

} // namespace Snowglobe::SnowCore::ESC
