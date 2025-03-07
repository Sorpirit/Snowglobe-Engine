#pragma once
#include "DependencyManager.hpp"
#include "ECS/Entity.hpp"
#include "Engine.hpp"

#include <memory>

namespace Snowglobe::Core
{
struct PrefabAssetData
{
    std::shared_ptr<ECS::Entity> DetachedEntity = nullptr;
};
} // namespace Snowglobe::Core

inline void ProcessPrefabAssetData(Snowglobe::Core::AssetManager* manger, std::filesystem::path& assetPath)
{
    auto detached = DI->Resolve<Snowglobe::Engine::Engine>()->GetEntityManager()->CreateEntityDetached();
    // TODO:
    // Deserialize entity into the detached
    manger->RegisterAsset(assetPath.filename().string(), Snowglobe::Core::PrefabAssetData{std::move(detached)});
}

template <> inline void SetupAssetProcessor<Snowglobe::Core::PrefabAssetData>(Snowglobe::Core::AssetManager* manager)
{
    manager->RegisterAssetProcessor(".prefab", ProcessPrefabAssetData);
}