#include "SceneSerializationTest.hpp"

#include "Serialization/SerializationAPI.hpp"
#include "Serialization/JsonSerialization.hpp"
#include "Collider2DComponent.hpp"
#include "ECS/ECSTest/ECSTest.hpp"
#include "Physics2DComponent.hpp"
#include "SpriteAnimationComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "TransformComponent.hpp"

template <> void CustomProp<Snowglobe::Core::ECS::Entity>(Snowglobe::Core::Serialization::SerializationAPI* api,
                                              Snowglobe::Core::ECS::Entity* value, uint32_t metaFlags)
{
    std::string name = value->GetName();
    std::string tagName = value->GetTag().GetName();
    api->BaseProperty("Name", name);
    api->BaseProperty("Tag", tagName);
    std::vector<Snowglobe::Core::ECS::Component*> components;
    value->ListAttachedComponents(components);
    api->Property("Components", components);
}


namespace Snowglobe
{

class CustomSceneEntity
{
  public:
    CustomSceneEntity(const std::shared_ptr<Core::ECS::EntityManagerBase>& entity_manager)
        : _entityManager(entity_manager)
    {
    }

    template<typename CComponent>
    CComponent* Deserialize(Core::Serialization::Deserializer* des);

  private:
    std::shared_ptr<Core::ECS::EntityManagerBase> _entityManager;
    std::shared_ptr<Core::ECS::Entity> _entity;
};

template<>
Core::ECS::Entity* CustomSceneEntity::Deserialize<Core::ECS::Entity>(Core::Serialization::Deserializer* s)
{
    std::string name;
    std::string tagName;
    s->BaseProperty("Name", name);
    s->BaseProperty("Tag", tagName);
    Core::ECS::Tag tag = Core::ECS::TagManager::GetInstance().GetTag(tagName);

    _entity = _entityManager->CreateEntity(tag);
    _entity->SetName(name);
    s->DeserializeArray("Components");
    return _entity.get();
}

template<>
Core::TransformComponent* CustomSceneEntity::Deserialize<Core::TransformComponent>(Core::Serialization::Deserializer* s)
{
    auto comp = _entity->AddOrGetComponent<Core::TransformComponent>();
    s->Property(*comp);
    return comp;
}

template<>
Render::SpriteRenderComponent* CustomSceneEntity::Deserialize<Render::SpriteRenderComponent>(Core::Serialization::Deserializer* s)
{
    auto comp = _entity->AddOrGetComponent<Render::SpriteRenderComponent>();
    s->Property(*comp);
    return comp;
}

template<>
Render::SpriteAnimationComponent* CustomSceneEntity::Deserialize<Render::SpriteAnimationComponent>(Core::Serialization::Deserializer* s)
{
    auto comp = _entity->AddOrGetComponent<Render::SpriteAnimationComponent>();
    s->Property(*comp);
    return comp;
}

template<>
Engine::Physics2DComponent* CustomSceneEntity::Deserialize<Engine::Physics2DComponent>(Core::Serialization::Deserializer* s)
{
    auto comp = _entity->AddOrGetComponent<Engine::Physics2DComponent>();
    s->Property(*comp);
    return comp;
}

template<>
Engine::Collider2DComponent* CustomSceneEntity::Deserialize<Engine::Collider2DComponent>(Core::Serialization::Deserializer* s)
{
    auto comp = _entity->AddOrGetComponent<Engine::Collider2DComponent>();
    s->Property(*comp);
    return comp;
}

void SceneSerializationTest::Init()
{
    RuntimeTest::Init();

    std::stringstream ss;
    _fileSystem->TryReadTextFile(Core::SnowFileHandle("SceneTest.json"), ss);
    ss >> _sceneJson;
    LoadScene();
}

void SceneSerializationTest::Run()
{
    if (_input->IsKeyPressed(Core::Key::F1))
    {
        // Save
        SaveScene();
    }

    if (_input->IsKeyPressed(Core::Key::F2))
    {
        // Load
        LoadScene();
    }

    if (_input->IsKeyPressed(Core::Key::F3))
    {
        // DestoryScene
        _engine->GetLifetimeSystem().Expire(_sceneLifetime);
        _sceneLifetime = {0};
    }
}
void SceneSerializationTest::SaveScene()
{
    auto manager = _engine->GetEntityManager();
    Core::Serialization::JsonWriter jwriter;
    jwriter.RegisterCustomProperty(CustomProp<Core::ECS::Entity>);
    jwriter.RegisterCustomProperty(CustomProp<Core::TransformComponent>);
    jwriter.RegisterCustomProperty(CustomProp<Render::SpriteRenderComponent>);
    jwriter.RegisterCustomProperty(CustomProp<Render::SpriteAnimationComponent>);
    jwriter.RegisterCustomProperty(CustomProp<Engine::Physics2DComponent>);
    jwriter.RegisterCustomProperty(CustomProp<Engine::Collider2DComponent>);
    jwriter.RegisterCustomProperty(CustomProp<glm::vec2>);
    jwriter.RegisterCustomProperty(CustomProp<glm::vec3>);
    jwriter.RegisterCustomProperty(CustomProp<glm::vec4>);
    jwriter.RegisterCustomProperty(CustomProp<glm::ivec2>);
    jwriter.RegisterCustomProperty(CustomProp<glm::ivec3>);
    jwriter.RegisterCustomProperty(CustomProp<glm::ivec4>);
    jwriter.RegisterCustomProperty(CustomProp<Core::SnowFileHandle>);
    jwriter.Property("Entities", manager->GetAllEntities());

    _sceneJson = jwriter.GetResult();
    std::cout << _sceneJson.dump(4) << std::endl;
}
void SceneSerializationTest::LoadScene()
{
    if (_sceneLifetime.ID != 0)
        _engine->GetLifetimeSystem().Expire(_sceneLifetime);

    auto manager = _engine->GetEntityManager();
    _sceneLifetime = _engine->GetLifetimeSystem().Register("TempScene");
    auto sceneManager = std::make_shared<Core::ECS::SceneEntityManager>(manager, _sceneLifetime);
    CustomSceneEntity customScene(sceneManager);

    Core::Serialization::JsonReader jreader(_sceneJson);
    // jreader.RegisterCustomProperty(CustomProp<Core::ECS::Entity>);
    jreader.RegisterCustomProperty(CustomProp<Core::TransformComponent>);
    jreader.RegisterCustomProperty(CustomProp<Render::SpriteRenderComponent>);
    jreader.RegisterCustomProperty(CustomProp<Render::SpriteAnimationComponent>);
    jreader.RegisterCustomProperty(CustomProp<Engine::Physics2DComponent>);
    jreader.RegisterCustomProperty(CustomProp<Engine::Collider2DComponent>);
    jreader.RegisterCustomProperty(CustomProp<glm::vec2>);
    jreader.RegisterCustomProperty(CustomProp<glm::vec3>);
    jreader.RegisterCustomProperty(CustomProp<glm::vec4>);
    jreader.RegisterCustomProperty(CustomProp<glm::ivec2>);
    jreader.RegisterCustomProperty(CustomProp<glm::ivec3>);
    jreader.RegisterCustomProperty(CustomProp<glm::ivec4>);
    jreader.RegisterCustomProperty(CustomProp<Core::SnowFileHandle>);
    jreader.RegisterCustomDeserializer<Core::ECS::Entity, CustomSceneEntity>(&customScene);
    jreader.RegisterCustomDeserializer<Core::TransformComponent, CustomSceneEntity>(&customScene);
    jreader.RegisterCustomDeserializer<Render::SpriteRenderComponent, CustomSceneEntity>(&customScene);
    jreader.RegisterCustomDeserializer<Render::SpriteAnimationComponent, CustomSceneEntity>(&customScene);
    jreader.RegisterCustomDeserializer<Engine::Physics2DComponent, CustomSceneEntity>(&customScene);
    jreader.RegisterCustomDeserializer<Engine::Collider2DComponent, CustomSceneEntity>(&customScene);
    jreader.DeserializeArray<Core::ECS::Entity>("Entities");
}

} // namespace Snowglobe