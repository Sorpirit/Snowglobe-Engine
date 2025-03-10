#pragma once
#include "AssetManager.hpp"
#include "FileSystem.hpp"

namespace Snowglobe::Core
{
struct SceneAssetData
{
    SnowFileHandle SceneFile;
};
} // namespace Snowglobe::Core

template <> inline void SetupAssetProcessor<Snowglobe::Core::SceneAssetData>(Snowglobe::Core::AssetManager* manager)
{
    manager->RegisterAssetProcessor(".scene",
        [](Snowglobe::Core::AssetManager* manger, const std::filesystem::path& assetPath) {
        manger->RegisterAsset(assetPath.filename().string(), Snowglobe::Core::SceneAssetData{assetPath});
    });
}