#pragma once
#include "ECS/Component.hpp"
#include "ECS/EntityManager.hpp"
#include "Serialization/Deserialize.hpp"

#include <memory>

namespace Snowglobe::Engine
{

class EntityFactory
{
  public:
    EntityFactory(const std::shared_ptr<Core::ECS::EntityManagerBase>& entity_manager) : _entityManager(entity_manager)
    {
    }
    EntityFactory(const std::shared_ptr<Core::ECS::EntityManagerBase>& entity_manager,
                  const std::shared_ptr<Core::ECS::Entity>& current_entity)
        : _entityManager(entity_manager), _currentEntity(current_entity)
    {
    }

    template <typename CComponent> CComponent* Deserialize(Core::Serialization::Deserializer* des);

    template <Core::ECS::ComponentBased CComponent> CComponent* Deserialize(Core::Serialization::Deserializer* des)
    {
        auto comp = _currentEntity->AddOrGetComponent<CComponent>();
        des->Property(*comp);
        return comp;
    }

  private:
    std::shared_ptr<Core::ECS::EntityManagerBase> _entityManager;
    std::shared_ptr<Core::ECS::Entity> _currentEntity;
};
} // namespace Snowglobe::Engine

template <>
inline Snowglobe::Core::ECS::Entity* Snowglobe::Engine::EntityFactory::Deserialize<Snowglobe::Core::ECS::Entity>(
    Core::Serialization::Deserializer* s)
{
    std::string name;
    std::string tagName;
    s->BaseProperty("Name", name);
    s->BaseProperty("Tag", tagName);
    Core::ECS::Tag tag = Core::ECS::TagManager::GetInstance().GetTag(tagName);

    _currentEntity = _entityManager->CreateEntity(tag);
    _currentEntity->SetName(name);
    s->DeserializeArray("Components");
    return _currentEntity.get();
}

template <>
inline void CustomPropertySerialization<Snowglobe::Core::ECS::Entity>(Snowglobe::Core::Serialization::SerializationAPI* api,
                                                               Snowglobe::Core::ECS::Entity* value, uint32_t metaFlags)
{
    std::string name = value->GetName();
    std::string tagName = value->GetTag().GetName();
    api->BaseProperty("Name", name);
    api->BaseProperty("Tag", tagName);
    std::vector<Snowglobe::Core::ECS::Component*> components;
    value->ListAttachedComponents(components);
    api->Property<std::vector<Snowglobe::Core::ECS::Component*>, true>("Components", components);
}
