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

template <> inline void SetupAssetProcessor<Snowglobe::Core::PrefabAssetData>(Snowglobe::Core::AssetManager* manager)
{
    std::weak_ptr const engine = DI->Resolve<Snowglobe::Engine::Engine>();
    manager->RegisterAssetProcessor(".prefab",
        [engine](Snowglobe::Core::AssetManager* manger, std::filesystem::path& assetPath) {
            if (auto engineRef = engine.lock())
            {
                auto detached = engineRef->GetEntityManager()->CreateEntityDetached();
                // TODO:
                // Deserialize entity into the detached
                manger->RegisterAsset(assetPath.filename().string(), Snowglobe::Core::PrefabAssetData{std::move(detached)});
            }
    });
}