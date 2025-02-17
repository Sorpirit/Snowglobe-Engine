#pragma once

#include "ECS/SystemManager.hpp"
#include "Engine.hpp"
#include "FileSystem.hpp"
#include "MaterialsData/TextureColorMaterialData.hpp"
#include "RenderSystem.hpp"

#define ASSIMP_STATIC
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Snowglobe::Engine {

class AssimpImporter {
public:
    AssimpImporter()
    {
        auto engine = DI->Resolve<Engine>();
        engine->GetSystemManager()->QuerySystem<Snowglobe::Render::RenderSystem>(_renderSystem);

        auto renderMaterial = _renderSystem->CreateMaterialInstance<Render::MaterialsData::TextureColorMaterialData>("AssimpImporter::CreateMesh");
        renderMaterial.Properties()->color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
        _defaultMaterial = &renderMaterial.GetMaterialBase();
    }

    std::vector<Render::MeshProxy*> LoadModel(const Core::SnowFileHandle& modelPath);

private:
    Render::MeshProxy* CreateMesh(aiMesh* assimpMesh, const aiScene* scene, const std::filesystem::path& modelDirectory);

    Render::RenderSystem* _renderSystem;
    Render::MaterialBase* _defaultMaterial;
    std::unordered_map<std::string, Render::Texture2DPtr> _textures;
};

} // Snowglobe
