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

inline void ProcessSceneAssetData(Snowglobe::Core::AssetManager* manger, std::filesystem::path& assetPath)
{
    manger->RegisterAsset(assetPath.filename().string(), Snowglobe::Core::SceneAssetData{assetPath});
}

template <> inline void SetupAssetProcessor<Snowglobe::Core::SceneAssetData>(Snowglobe::Core::AssetManager* manager)
{
    manager->RegisterAssetProcessor(".scene", ProcessSceneAssetData);
}