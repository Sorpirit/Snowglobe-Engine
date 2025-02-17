#include "AssimpImporter.hpp"

#include "CommonVertexLayouts.hpp"
#include "Engine.hpp"
#include "MaterialsData/TextureColorMaterialData.hpp"
#include "MaterialsData/TextureLitMaterialData.hpp"
#include "RenderSystem.hpp"

#include <iostream>
#include <queue>

namespace Snowglobe::Engine {
std::vector<Render::MeshProxy*> AssimpImporter::LoadModel(const Core::SnowFileHandle& modelPath)
{
    Assimp::Importer import;
    std::string path = modelPath.GetFullPath().string();
    std::vector<Render::MeshProxy*> model;

    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return model;
    }

    auto directoryPath = modelPath.GetFullPath().parent_path();
    std::queue<aiNode*> nodeQueue;
    nodeQueue.push(scene->mRootNode);
    while (!nodeQueue.empty())
    {
        auto node = nodeQueue.front();
        nodeQueue.pop();

        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            model.push_back(CreateMesh(mesh, scene, directoryPath));
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            nodeQueue.push(node->mChildren[i]);
        }
    }

    return model;
}

Render::MeshProxy* AssimpImporter::CreateMesh(aiMesh* assimpMesh, const aiScene* scene, const std::filesystem::path& modelDirectory)
{
    std::vector<Render::PositionNormalUVVertex> vertices(assimpMesh->mNumVertices);
    std::vector<uint32_t> indecies;
    const bool containsUVs = assimpMesh->HasTextureCoords(0);
    for (uint32_t i = 0; i < assimpMesh->mNumVertices; i++)
    {
        vertices[i] = {
            {assimpMesh->mVertices[i].x, assimpMesh->mVertices[i].y, assimpMesh->mVertices[i].z},
            {assimpMesh->mNormals[i].x, assimpMesh->mNormals[i].y, assimpMesh->mNormals[i].z}, {}};

        if (containsUVs)
        {
            vertices[i].uv = {assimpMesh->mTextureCoords[0][i].x, assimpMesh->mTextureCoords[0][i].y};
        }
    }

    for (uint32_t i = 0; i < assimpMesh->mNumFaces; i++)
    {
        aiFace& face = assimpMesh->mFaces[i];
        indecies.insert(indecies.end(), face.mIndices, face.mIndices + face.mNumIndices);
    }

    auto* vertexBuffer = _renderSystem->AllocateVertexBufferPtr(vertices);
    auto* indeciesBuffer = _renderSystem->AllocateIndexBufferPtr(indecies);
    auto mesh = _renderSystem->CreateMeshProxy(*vertexBuffer, *indeciesBuffer, "AssimpImporter::CreateMesh");

    Render::MaterialBase* materialBasePtr = _defaultMaterial;
    if (assimpMesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[assimpMesh->mMaterialIndex];
        assert(material->GetTextureCount(aiTextureType_DIFFUSE) <= 1);
        assert(material->GetTextureCount(aiTextureType_SPECULAR) <= 1);

        auto renderMaterial = _renderSystem->CreateMaterialInstance<Render::MaterialsData::TextureLitMaterialData>("AssimpImporter::CreateMesh");
        renderMaterial.Properties()->emissivePower = 0;
        auto textureDesc = Snowglobe::Render::Texture2DDescriptor{
            Snowglobe::Render::TextureWrap::Repeat,
            Snowglobe::Render::TextureWrap::Repeat,
            Snowglobe::Render::TextureFilter::Linear,
            Snowglobe::Render::TextureFilter::Linear};

        aiString str;
        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
            auto handle = modelDirectory / str.C_Str();

            if (!_textures.contains(str.C_Str()))
            {
                auto diffuseFileTexture = Snowglobe::Core::FileSystem::LoadTexture(handle);
                auto diffuse2D = _renderSystem->CreateTexture2D(*diffuseFileTexture, textureDesc);
                _textures[str.C_Str()] = diffuse2D;
            }

            renderMaterial.Properties()->diffuseTexture = _textures[str.C_Str()];
        }

        if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
        {
            material->GetTexture(aiTextureType_SPECULAR, 0, &str);

            if (!_textures.contains(str.C_Str()))
            {
                auto specularFileTexture = Snowglobe::Core::FileSystem::LoadTexture(modelDirectory / str.C_Str());
                auto specular2D = _renderSystem->CreateTexture2D(*specularFileTexture, textureDesc);
                _textures[str.C_Str()] = specular2D;
            }

            renderMaterial.Properties()->specularTexture = _textures[str.C_Str()];
        }

        if (material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) > 0)
        {
            material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &str);

            if (!_textures.contains(str.C_Str()))
            {
                auto ambientOcclusionFileTexture = Snowglobe::Core::FileSystem::LoadTexture(modelDirectory / str.C_Str());
                auto ambientOcclusion2D = _renderSystem->CreateTexture2D(*ambientOcclusionFileTexture, textureDesc);
                _textures[str.C_Str()] = ambientOcclusion2D;
            }

            renderMaterial.Properties()->aoTexture = _textures[str.C_Str()];
        }

        materialBasePtr = &renderMaterial.GetMaterialBase();
    }

    if (materialBasePtr != nullptr)
        mesh->SetMaterial(*materialBasePtr);

    return mesh;
}

} // Snowglobe