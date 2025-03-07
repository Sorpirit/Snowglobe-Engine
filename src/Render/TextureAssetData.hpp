#pragma once
#include "FileTexture.hpp"
#include "Texture2DPtr.hpp"

namespace Snowglobe::Render
{
struct TextureAssetData
{
    std::shared_ptr<Core::FileTexture> FiletTexture = nullptr;
    Texture2DDescriptor Descriptor;
    Texture2DPtr RenderTexture;
};
}

