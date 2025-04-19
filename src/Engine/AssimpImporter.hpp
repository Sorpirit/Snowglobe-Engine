#pragma once

#define ASSIMP_STATIC
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "MeshProxy.hpp"
#include "RenderSystem.hpp"
#include "FileSystem.hpp"

namespace Snowglobe::Engine {

class AssimpImporter {
public:
    AssimpImporter();

    std::vector<Render::MeshProxy*> LoadModel(const Core::SnowFileHandle& modelPath);

private:
    Render::MeshProxy* CreateMesh(aiMesh* assimpMesh, const aiScene* scene, const std::filesystem::path& modelDirectory);

    Render::RenderSystem* _renderSystem;
    Render::MaterialBase* _defaultMaterial;
    std::unordered_map<std::string, Render::Texture2DPtr> _textures;
};

} // Snowglobe
