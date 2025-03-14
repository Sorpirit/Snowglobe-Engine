#include "SceneManager.hpp"

#include "Assets/SceneAssetData.hpp"
#include "Collider2DComponent.hpp"
#include "CoreTypesSerialization.hpp"
#include "ECS/Entity.hpp"
#include "ECS/EntityManager.hpp"
#include "Engine.hpp"
#include "EngineTypesSerialization.hpp"
#include "EntityFactory.hpp"
#include "FileSystem.hpp"
#include "Physics2DComponent.hpp"
#include "RenderTypesSerialization.hpp"
#include "Serialization/JsonSerialization.hpp"
#include "SpriteAnimationComponent.hpp"
#include "SpriteRenderComponent.hpp"
#include "TransformComponent.hpp"

#include <iostream>
#include <ostream>

namespace Snowglobe::Engine
{
void SceneManager::LoadScene(const std::string& tag)
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

    Core::CoreTypesSerialization::RegisterCustomTypes(jreader);
    Core::CoreTypesSerialization::RegisterCustomTypesDeserialization(jreader);
    Render::RenderTypesSerialization::RegisterCustomTypes(jreader);
    EngineTypesSerialization::RegisterCustomTypes(jreader);
    factory.RegisterDeserializers(jreader);

    jreader.DeserializeArray<Core::ECS::Entity>("Entities");
}
void SceneManager::SaveScene(const std::string& tag)
{
    if (_currentSceneLifetime == Core::ECS::DefaultLifetime)
    {
        assert(false && "No scene is loaded");
        return;
    }

    auto manager = DI->Resolve<Engine>()->GetEntityManager();
    Core::Serialization::JsonWriter jwriter;

    Core::CoreTypesSerialization::RegisterCustomTypes(jwriter);
    Core::CoreTypesSerialization::RegisterCustomTypesSerialization(jwriter);
    Render::RenderTypesSerialization::RegisterCustomTypes(jwriter);
    EngineTypesSerialization::RegisterCustomTypes(jwriter);
    EngineTypesSerialization::RegisterCustomTypesSerialization(jwriter);

    jwriter.Property("Entities", manager->GetEntitiesWithLifetime(_currentSceneLifetime));

    auto scene = DI->Resolve<Core::AssetManager>()->Get<Core::SceneAssetData>(tag);
    std::stringstream ss;
    ss << jwriter.GetResult().dump(4);
    DI->Resolve<Core::FileSystem>()->TryWriteTextFile(scene->GetData().SceneFile, ss);
}

void SceneManager::UnloadScene(const std::string& tag)
{
    DI->Resolve<Engine>()->GetLifetimeSystem().Expire(_currentSceneLifetime);
}

} // namespace Snowglobe::Engine