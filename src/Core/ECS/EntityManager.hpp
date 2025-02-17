#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Entity.hpp"
#include "Tag.hpp"

template <> struct std::hash<std::pair<Snowglobe::Core::ECS::Lifetime, Snowglobe::Core::ECS::Tag>>
{
    std::size_t operator()(const std::pair<Snowglobe::Core::ECS::Lifetime, Snowglobe::Core::ECS::Tag>& pair) const noexcept
    {
        size_t lifetimeSeed = std::hash<Snowglobe::Core::ECS::Lifetime>{}(pair.first);
        size_t tagSeed = std::hash<Snowglobe::Core::ECS::Tag>{}(pair.second);
        return lifetimeSeed ^ (tagSeed << 1);
    }
};

namespace Snowglobe::Core::ECS
{

/// @brief Base class for EntityManagers. Provides the basic interface for creating and managing entities and not
/// dependent on the EntityData type.
class EntityManagerBase
{
  public:
    virtual ~EntityManagerBase() = default;

    /// @brief Create a new entity with the attached lifetime and given tag.
    /// @return Shared pointer to the created entity. All the entities are stored in the EntityManager.
    virtual std::shared_ptr<Entity> CreateEntity(Lifetime lifetime, Tag tag = Tags::Default()) = 0;

    /// @brief Create a new entity with the attached lifetime and given tag.
    /// @return Shared pointer to the created entity. All the entities are stored in the EntityManager.
    virtual std::shared_ptr<Entity> CreateEntity(Tag tag = Tags::Default()) = 0;

    /// @brief Get all entities with the given tag.
    /// @return Vector of shared pointers to the entities with the given tag.
    virtual std::vector<std::shared_ptr<Entity>>& GetEntitiesWithTag(Tag tag) = 0;

    /// @brief Get all entities with the attached lifetime.
    /// @return Vector of shared pointers to the entities with the attached lifetime.
    virtual std::vector<std::shared_ptr<Entity>>& GetEntitiesWithLifetime(Lifetime lifetime) = 0;

    /// @brief Get all entities with the attached lifetime and tag.
    /// @return Vector of shared pointers to the entities with the attached lifetime.
    virtual std::vector<std::shared_ptr<Entity>>& GetEntitiesWithLifetimeAndTag(Lifetime lifetime, Tag tag) = 0;

    /// @brief Get all entities that are managed by the EntityManager.
    /// @return Vector of shared pointers to all entities.
    virtual std::vector<std::shared_ptr<Entity>>& GetAllEntities() = 0;

    /// @brief Updates the EntityManager. This will update all the entities, destroy and detach the components from the
    /// entities that marked for destruction. Also, it will add the new entities to the EntityManager. This function
    /// should be called once per frame at the start or at the end of the frame.
    virtual void Update() = 0;
};

/// @brief EntityManager class that manages entities with the given EntityData type.
template <class TEntityData> class EntityManager final : public EntityManagerBase
{
  public:
    std::shared_ptr<Entity> CreateEntity(Lifetime lifetime, Tag tag = Tags::Default()) override
    {
        auto entityData = std::make_unique<TEntityData>();
        auto entity = std::make_shared<Entity>(std::move(entityData), _nextId, tag, lifetime,
                                               "Entity_" + std::to_string(_nextId));
        _toAddEntities.push_back(entity);
        _nextId++;
        return entity;
    }

    std::shared_ptr<Entity> CreateEntity(Tag tag = Tags::Default()) override
    {
        return CreateEntity(DefaultLifetime, tag);
    }

    std::vector<std::shared_ptr<Entity>>& GetEntitiesWithTag(Tag tag) override { return _entityTagMap[tag]; }
    std::vector<std::shared_ptr<Entity>>& GetEntitiesWithLifetime(Lifetime lifetime) override { return _entityLifetimeMap[lifetime]; }
    std::vector<std::shared_ptr<Entity>>& GetEntitiesWithLifetimeAndTag(Lifetime lifetime, Tag tag) override { return _entityLifetimeTagMap[std::make_pair(lifetime, tag)]; }
    std::vector<std::shared_ptr<Entity>>& GetAllEntities() override { return _entities; }

    void Update() override
    {
        // In entity update we detach all components that was removed in the previous frame.
        // If the entity is destroyed, we detach all components.
        for (const auto& entity : _entities)
        {
            entity->Update();
        }

        for (auto& entitiesVec : _entityTagMap | std::views::values)
        {
            std::erase_if(entitiesVec, [](auto& entity) { return entity->IsDestroyed(); });
        }
        for (auto& entitiesVec : _entityLifetimeMap | std::views::values)
        {
            std::erase_if(entitiesVec, [](auto& entity) { return entity->IsDestroyed(); });
        }
        for (auto& entitiesVec : _entityLifetimeTagMap | std::views::values)
        {
            std::erase_if(entitiesVec, [](auto& entity) { return entity->IsDestroyed(); });
        }
        std::erase_if(_entities, [](auto& entity) {
            assert((!entity->IsDestroyed() || entity.use_count() <= 1) &&
                   "Unable to destroy entity because it is referenced somewhere else");
            return entity->IsDestroyed();
        });

        for (auto& entity : _toAddEntities)
        {
            _entities.push_back(entity);
            _entityTagMap[entity->GetTag()].push_back(entity);
            _entityLifetimeMap[entity->GetLifetime()].push_back(entity);
            _entityLifetimeTagMap[std::make_pair(entity->GetLifetime(), entity->GetTag())].push_back(entity);
        }

        _toAddEntities.clear();
    }

  private:
    std::vector<std::shared_ptr<Entity>> _entities;
    std::unordered_map<Tag, std::vector<std::shared_ptr<Entity>>> _entityTagMap;
    std::unordered_map<Lifetime, std::vector<std::shared_ptr<Entity>>> _entityLifetimeMap;
    std::unordered_map<std::pair<Lifetime, Tag>, std::vector<std::shared_ptr<Entity>>> _entityLifetimeTagMap;

    size_t _nextId = 0;
    std::vector<std::shared_ptr<Entity>> _toAddEntities;
};

class SceneEntityManager final : public EntityManagerBase
{
 public:
    SceneEntityManager(std::shared_ptr<EntityManagerBase> manager, Lifetime defaultLifetime) : _manager(std::move(manager)), _defaultLifetime(defaultLifetime) {}

    std::shared_ptr<Entity> CreateEntity(Lifetime lifetime, Tag tag = Tags::Default()) override { return _manager->CreateEntity(lifetime, tag); }
    std::shared_ptr<Entity> CreateEntity(Tag tag = Tags::Default()) override { return _manager->CreateEntity(_defaultLifetime, tag); }

    std::vector<std::shared_ptr<Entity>>& GetEntitiesWithTag(Tag tag) override { return _manager->GetEntitiesWithLifetimeAndTag(_defaultLifetime, tag); }
    std::vector<std::shared_ptr<Entity>>& GetEntitiesWithLifetime(Lifetime lifetime) override { return _manager->GetEntitiesWithLifetime(lifetime); }
    std::vector<std::shared_ptr<Entity>>& GetEntitiesWithLifetimeAndTag(Lifetime lifetime, Tag tag) override { return _manager->GetEntitiesWithLifetimeAndTag(lifetime, tag);  }
    std::vector<std::shared_ptr<Entity>>& GetAllEntities() override { return _manager->GetEntitiesWithLifetime(_defaultLifetime); }

    void Update() override { _manager->Update(); }

    Lifetime GetLifetime() const { return _defaultLifetime; }
 private:
    std::shared_ptr<EntityManagerBase> _manager;
    Lifetime _defaultLifetime = Lifetime{0};
};

} // namespace Snowglobe::Core::ECS