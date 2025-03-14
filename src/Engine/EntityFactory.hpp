#pragma once
#include "ECS/Component.hpp"
#include "ECS/EntityManager.hpp"
#include "Serialization/Deserialize.hpp"
#include "SpriteAnimationComponent.hpp"

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

    void RegisterDeserializers(Core::Serialization::Deserializer& des)
    {
        RegisterDeserializationFunc<Core::ECS::Entity>(des);
        RegisterDeserializationFunc<Core::TransformComponent>(des);
        RegisterDeserializationFunc<Render::SpriteRenderComponent>(des);
        RegisterDeserializationFunc<Render::SpriteAnimationComponent>(des);
        RegisterDeserializationFunc<Physics2DComponent>(des);
        RegisterDeserializationFunc<Collider2DComponent>(des);
    }

  private:
    std::shared_ptr<Core::ECS::EntityManagerBase> _entityManager;
    std::shared_ptr<Core::ECS::Entity> _currentEntity;

    template <typename CComponent> CComponent* Deserialize(Core::Serialization::Deserializer* des);

    template <Core::ECS::ComponentBased CComponent> CComponent* Deserialize(Core::Serialization::Deserializer* des)
    {
        auto comp = _currentEntity->AddOrGetComponent<CComponent>();
        des->Property(*comp);
        return comp;
    }

    template <typename T> void RegisterDeserializationFunc(Core::Serialization::Deserializer& deserializer)
    {
        deserializer.RegisterCustomDeserializer<T>([&](Core::Serialization::Deserializer* deserializer) -> T& {
            auto t = this->Deserialize<T>(deserializer);
            return *t;
        });
    }
};
} // namespace Snowglobe::Engine

template <>
inline Snowglobe::Core::ECS::Entity* Snowglobe::Engine::EntityFactory::Deserialize<Snowglobe::Core::ECS::Entity>(
    Core::Serialization::Deserializer* s)
{
    std::string name;
    std::string tagName;
    s->Property("Name", name);
    s->Property("Tag", tagName);
    Core::ECS::Tag tag = Core::ECS::TagManager::GetInstance().GetTag(tagName);

    _currentEntity = _entityManager->CreateEntity(tag);
    _currentEntity->SetName(name);
    s->DeserializeArray("Components");
    return _currentEntity.get();
}