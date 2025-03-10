#include "SceneManager.hpp"

#include "Assets/SceneAssetData.hpp"
#include "Collider2DComponent.hpp"
#include "ECS/Entity.hpp"
#include "ECS/EntityManager.hpp"
#include "Engine.hpp"
#include "EntityFactory.hpp"
#include "FileSystem.hpp"
#include "Physics2DComponent.hpp"
#include "Serialization/JsonSerialization.hpp"
#include "SpriteAnimationComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "TransformComponent.hpp"

#include <iostream>
#include <ostream>

namespace Snowglobe::Engine
{
void SceneManager::LoadScene(std::string tag)
{
    if (_currentSceneLifetime != Core::ECS::DefaultLifetime)
        UnloadScene(tag);

    auto scene = DI->Resolve<Core::AssetManager>()->Get<Core::SceneAssetData>(tag);

    nlohmann::json sceneData;
    std::stringstream ss;
    DI->Resolve<Core::FileSystem>()->TryReadTextFile(scene->GetData().SceneFile, ss);
    ss >> sceneData;
    auto engine = DI->Resolve<Engine>();
    auto manager = engine->GetEntityManager();
    _currentSceneLifetime = engine->GetLifetimeSystem().Register(tag);
    auto sceneManager = std::make_shared<Core::ECS::SceneEntityManager>(manager, _currentSceneLifetime);
    EntityFactory factory(sceneManager);

    Core::Serialization::JsonReader jreader(sceneData);
    jreader.RegisterCustomProperty<Core::TransformComponent>();
    jreader.RegisterCustomProperty<Render::SpriteRenderComponent>();
    jreader.RegisterCustomProperty<Render::SpriteAnimationComponent>();
    jreader.RegisterCustomProperty<Physics2DComponent>();
    jreader.RegisterCustomProperty<Collider2DComponent>();
    jreader.RegisterCustomProperty<glm::vec2>();
    jreader.RegisterCustomProperty<glm::vec3>();
    jreader.RegisterCustomProperty<glm::vec4>();
    jreader.RegisterCustomProperty<glm::ivec2>();
    jreader.RegisterCustomProperty<glm::ivec3>();
    jreader.RegisterCustomProperty<glm::ivec4>();
    jreader.RegisterCustomProperty<Core::SnowFileHandle>();
    jreader.RegisterCustomPropertyDeserialization<Core::AssetBase>();
    jreader.RegisterCustomDeserializer<Core::ECS::Entity, EntityFactory>(&factory);
    jreader.RegisterCustomDeserializer<Core::TransformComponent, EntityFactory>(&factory);
    jreader.RegisterCustomDeserializer<Render::SpriteRenderComponent, EntityFactory>(&factory);
    jreader.RegisterCustomDeserializer<Render::SpriteAnimationComponent, EntityFactory>(&factory);
    jreader.RegisterCustomDeserializer<Physics2DComponent, EntityFactory>(&factory);
    jreader.RegisterCustomDeserializer<Collider2DComponent, EntityFactory>(&factory);

    jreader.DeserializeArray<Core::ECS::Entity>("Entities");
}
void SceneManager::SaveScene(std::string tag)
{
    if (_currentSceneLifetime == Core::ECS::DefaultLifetime)
    {
        assert(false && "No scene is loaded");
        return;
    }

    auto manager = DI->Resolve<Engine>()->GetEntityManager();
    Core::Serialization::JsonWriter jwriter;

    jwriter.RegisterCustomProperty<Core::TransformComponent>();
    jwriter.RegisterCustomProperty<Render::SpriteRenderComponent>();
    jwriter.RegisterCustomProperty<Physics2DComponent>();
    jwriter.RegisterCustomProperty<Render::SpriteAnimationComponent>();
    jwriter.RegisterCustomProperty<Collider2DComponent>();
    jwriter.RegisterCustomProperty<glm::vec2>();
    jwriter.RegisterCustomProperty<glm::vec3>();
    jwriter.RegisterCustomProperty<glm::vec4>();
    jwriter.RegisterCustomProperty<glm::ivec2>();
    jwriter.RegisterCustomProperty<glm::ivec3>();
    jwriter.RegisterCustomProperty<glm::ivec4>();
    jwriter.RegisterCustomProperty<Core::SnowFileHandle>();
    jwriter.RegisterCustomPropertySerialization<Core::ECS::Entity>();
    jwriter.Property("Entities", manager->GetEntitiesWithLifetime(_currentSceneLifetime));

    auto scene = DI->Resolve<Core::AssetManager>()->Get<Core::SceneAssetData>(tag);
    std::stringstream ss;
    ss << jwriter.GetResult().dump(4);
    DI->Resolve<Core::FileSystem>()->TryWriteTextFile(scene->GetData().SceneFile, ss);
}

void SceneManager::UnloadScene(std::string tag) { DI->Resolve<Engine>()->GetLifetimeSystem().Expire(_currentSceneLifetime); }

} // namespace Snowglobe::Engine