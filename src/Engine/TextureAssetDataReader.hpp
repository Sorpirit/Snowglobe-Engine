#pragma once
#include "Assets/AssetManager.hpp"
#include "CoreTypesSerialization.hpp"
#include "Engine.hpp"
#include "EngineTypesSerialization.hpp"
#include "EntityFactory.hpp"
#include "RenderSystem.hpp"
#include "RenderTypesSerialization.hpp"
#include "Serialization/JsonSerialization.hpp"
#include "Texture2DPtr.hpp"
#include "TextureAssetData.hpp"

inline void ProcessTextureAssetData(Snowglobe::Core::AssetManager* manger, const std::filesystem::path& assetPath)
{
    auto ftexture = Snowglobe::Core::FileSystem::LoadTexture(assetPath);
    auto textureDesc = Snowglobe::Render::Texture2DDescriptor{
        Snowglobe::Render::TextureWrap::Repeat, Snowglobe::Render::TextureWrap::Repeat,
        Snowglobe::Render::TextureFilter::Nearest, Snowglobe::Render::TextureFilter::Nearest};
    Snowglobe::Render::RenderSystem* renderPtr = nullptr;
    DI->Resolve<Snowglobe::Engine::Engine>()->GetSystemManager()->QuerySystem<Snowglobe::Render::RenderSystem>(
        renderPtr);
    Snowglobe::Render::Texture2DPtr rTexture = renderPtr->CreateTexture2D(*ftexture, textureDesc);

    manger->RegisterAsset(assetPath.filename().string(),
                          Snowglobe::Render::TextureAssetData{ftexture, textureDesc, rTexture});
}

template <> inline void SetupAssetProcessor<Snowglobe::Render::TextureAssetData>(Snowglobe::Core::AssetManager* manager)
{
    manager->RegisterAssetProcessor(".png", ProcessTextureAssetData);
}

template <> inline void SetupAssetProcessor<Snowglobe::Core::PrefabAssetData>(Snowglobe::Core::AssetManager* manager)
{
    using namespace Snowglobe;
    using namespace Snowglobe::Engine;

    std::weak_ptr const engine = DI->Resolve<Snowglobe::Engine::Engine>();
    std::shared_ptr const fileSystem = DI->Resolve<Core::FileSystem>();
    manager->RegisterAssetProcessor(".prefab", [engine, fileSystem](Core::AssetManager* manger,
                                                       const std::filesystem::path& assetPath) {
        if (auto engineRef = engine.lock())
        {
            nlohmann::json prefabData;
            std::stringstream ss;
            fileSystem->TryReadTextFile(assetPath, ss);
            ss >> prefabData;

            std::string name = prefabData["Name"];
            std::string tagName = prefabData["Tag"];
            Core::ECS::Tag tag = Core::ECS::TagManager::GetInstance().GetTag(tagName);

            auto manager = engineRef->GetEntityManager();
            auto detached = manager->CreateEntityDetached(tag);
            detached->SetName(name);
            EntityFactory factory(manager, detached);

            Core::Serialization::JsonReader jreader(prefabData);

            Core::CoreTypesSerialization::RegisterCustomTypes(jreader);
            Core::CoreTypesSerialization::RegisterCustomTypesDeserialization(jreader);
            Render::RenderTypesSerialization::RegisterCustomTypes(jreader);
            EngineTypesSerialization::RegisterCustomTypes(jreader);
            factory.RegisterDeserializers(jreader);

            jreader.DeserializeArray("Components");

            manger->RegisterAsset(assetPath.filename().string(), Core::PrefabAssetData{std::move(detached)});
        }
    });
}

template <> inline void SetupAssetProcessor<Snowglobe::Render::SpriteAssetData>(Snowglobe::Core::AssetManager* manager)
{
    using namespace Snowglobe;
    using namespace Snowglobe::Engine;

    std::weak_ptr const engine = DI->Resolve<Snowglobe::Engine::Engine>();
    std::shared_ptr const fileSystem = DI->Resolve<Core::FileSystem>();
    manager->RegisterAssetProcessor(".sprite", [engine, fileSystem](Core::AssetManager* manger,
                                                       const std::filesystem::path& assetPath) {
        if (auto engineRef = engine.lock())
        {
            nlohmann::json prefabData;
            std::stringstream ss;
            fileSystem->TryReadTextFile(assetPath, ss);
            ss >> prefabData;


            Render::SpriteAssetData sprite{};
            manger->ResolveRef(prefabData["TextureAsset"]["Tag"], &sprite.TextureAsset);
            sprite.TileSize = {prefabData["TileSize"]["x"], prefabData["TileSize"]["y"]};
            sprite.TileCount = {prefabData["TileCount"]["x"], prefabData["TileCount"]["y"]};
            sprite.SpriteTotalCount = prefabData["SpriteTotalCount"];
            sprite.SpriteScale = {prefabData["SpriteScale"]["x"], prefabData["SpriteScale"]["y"]};
            sprite.CurrentSpriteCoord = {prefabData["CurrentSpriteCoord"]["x"], prefabData["CurrentSpriteCoord"]["y"]};

            manger->RegisterAsset(assetPath.filename().string(), sprite);
        }
    });
}