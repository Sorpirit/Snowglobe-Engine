#pragma once
#include "Assets/AssetManager.hpp"
#include "Engine.hpp"
#include "RenderSystem.hpp"
#include "Texture2DPtr.hpp"
#include "TextureAssetData.hpp"

inline void ProcessTextureAssetData(Snowglobe::Core::AssetManager* manger, std::filesystem::path& assetPath)
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