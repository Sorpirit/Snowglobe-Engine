#include "Basic3DTest.hpp"

#include "AssimpImporter.hpp"
#include "CommonVertexLayouts.hpp"
#include "MaterialsData/TextureColorMaterialData.hpp"
#include "MaterialsData/TextureLitOutlineMaterialData.hpp"
#include "MeshComponent.hpp"
#include "TransformComponent.hpp"
#include "../RenderOpenGL/LightsManager.hpp"

namespace Snowglobe
{

void Basic3DTest::Init()
{
    RuntimeTest::Init();

    auto manager = _engine->GetEntityManager();
    auto assets = DI->Resolve<Core::AssetManager>();

    auto grassTexture = assets->Get<Render::TextureAssetData>("container.png");
    auto diffuseTexture = assets->Get<Render::TextureAssetData>("container2.png");
    auto specularTexture = assets->Get<Render::TextureAssetData>("container2_specular.png");
    auto emissiveTexture = assets->Get<Render::TextureAssetData>("container2_emissive.png");

    std::vector<Render::PositionNormalUVVertex> cubeVerticesNormals;
    AddQuad(cubeVerticesNormals, glm::mat4x4(1.0f));
    AddQuad(cubeVerticesNormals, glm::rotate(glm::mat4x4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    AddQuad(cubeVerticesNormals, glm::rotate(glm::mat4x4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    AddQuad(cubeVerticesNormals, glm::rotate(glm::mat4x4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    AddQuad(cubeVerticesNormals, glm::rotate(glm::mat4x4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    AddQuad(cubeVerticesNormals, glm::rotate(glm::mat4x4(1.0f), glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    auto cubeNormalVB = _renderSystem->AllocateVertexBufferPtr(cubeVerticesNormals);

    std::vector<Render::PositionUVVertex> planeVertices;
    AddQuad(planeVertices, glm::mat4x4(1.0f));
    auto planeVB = _renderSystem->AllocateVertexBufferPtr(planeVertices);

    auto florMaterial = _renderSystem->CreateMaterialInstance<Render::MaterialsData::TextureLitMaterialData>();
    florMaterial.Properties()->diffuseTexture = diffuseTexture->GetData().RenderTexture;
    florMaterial.Properties()->specularTexture = specularTexture->GetData().RenderTexture;
    florMaterial.Properties()->emissiveTexture = emissiveTexture->GetData().RenderTexture;
    florMaterial.Properties()->specularPower = 32;

    auto containerMaterial = _renderSystem->CreateMaterialInstance<Render::MaterialsData::TextureLitOutlineMaterialData>();
    containerMaterial.Properties()->Material.diffuseTexture = diffuseTexture->GetData().RenderTexture;
    containerMaterial.Properties()->Material.specularTexture = specularTexture->GetData().RenderTexture;
    containerMaterial.Properties()->Material.emissiveTexture = emissiveTexture->GetData().RenderTexture;
    containerMaterial.Properties()->Material.specularPower = 32;
    containerMaterial.Properties()->outlineColor = glm::vec3(.8f, .3f, .1f);

    auto grassMaterial = _renderSystem->CreateMaterialInstance<Render::MaterialsData::TextureColorMaterialData>();
    grassMaterial.Properties()->texture = grassTexture->GetData().RenderTexture;

    auto florMesh = _renderSystem->CreateMeshProxy(*cubeNormalVB, "FlorMesh");
    florMesh->SetMaterial(florMaterial.GetMaterialBase());

    auto flor = manager->CreateEntity();
    flor->SetName("Flor");
    flor->AddComponent<Core::TransformComponent>(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(15.0f, 1.0f, 15.0f));
    flor->AddComponent<Engine::MeshComponent>(florMesh);

    auto cube1Mesh = _renderSystem->CreateMeshProxy(*cubeNormalVB, "cube1Mesh");
    cube1Mesh->SetMaterial(containerMaterial.GetMaterialBase());

    auto cube1 = manager->CreateEntity();
    cube1->SetName("Cube1");
    cube1->AddComponent<Core::TransformComponent>(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 90.0f, 45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    cube1->AddComponent<Engine::MeshComponent>(cube1Mesh);

    auto grassMesh = _renderSystem->CreateMeshProxy(*planeVB, "grassMesh");
    grassMesh->SetMaterial(grassMaterial.GetMaterialBase());

    auto grass = manager->CreateEntity();
    grass->SetName("Grass");
    grass->AddComponent<Core::TransformComponent>(glm::vec3(-2.0f, 0.0f, 1.0f), glm::vec3(180.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f));
    grass->AddComponent<Engine::MeshComponent>(grassMesh);

    auto grass2Mesh = _renderSystem->CreateMeshProxy(*planeVB, "grassMesh2");
    grass2Mesh->SetMaterial(grassMaterial.GetMaterialBase());

    auto grass2 = manager->CreateEntity();
    grass2->SetName("Grass");
    grass2->AddComponent<Core::TransformComponent>(glm::vec3(-1.0f, 0.0f, 3.0f), glm::vec3(180.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f));
    grass2->AddComponent<Engine::MeshComponent>(grass2Mesh);

    Snowglobe::Engine::AssimpImporter importer;
    auto meshList = importer.LoadModel(Snowglobe::Core::SnowFileHandle("Models/backpack/backpack.obj"));
    auto backpack = manager->CreateEntity();
    backpack->SetName("Backpack");
    backpack->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(0, 1, 4), glm::vec3(0.0f), glm::vec3(0.5f));
    backpack->AddComponent<Snowglobe::Engine::ModelComponent>(meshList);

    Snowglobe::Engine::AssimpImporter importer2;
    auto meshList2 = importer2.LoadModel(Snowglobe::Core::SnowFileHandle("Models/ornate_mirror_01_1k/ornate_mirror_01_1k.fbx"));
    auto mirror = manager->CreateEntity();
    mirror->SetName("Mirror");
    mirror->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(4, 1, 0), glm::vec3(0.0f), glm::vec3(0.5f));
    mirror->AddComponent<Snowglobe::Engine::ModelComponent>(meshList);

    auto directionalLight = manager->CreateEntity(Tags::Lights());
    directionalLight->SetName("DirectionalLight" );
    directionalLight->AddComponent<Core::TransformComponent>(glm::vec3(2, 2.0f, 1.0f), glm::vec3(45, 25, 0), glm::vec3(0.3f));
    directionalLight->AddComponent<RenderOpenGL::DirectionalLightComponent>();
}

void Basic3DTest::Run() {}

void Basic3DTest::AddQuad(std::vector<Render::PositionNormalUVVertex>& vertices, glm::mat4x4 transform)
{
    const Render::PositionUVVertex quad[6] = {
        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)},
    };

    for (int i = 0; i < 6; ++i)
    {
        vertices.push_back({transform * glm::vec4(quad[i].position, 1.0f), transform * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), quad[i].uv});
    }
}

void Basic3DTest::AddQuad(std::vector<Render::PositionUVVertex>& vertices, glm::mat4x4 transform)
{
    const Render::PositionUVVertex quad[6] = {
        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)},
    };

    for (int i = 0; i < 6; ++i)
    {
        vertices.push_back({transform * glm::vec4(quad[i].position, 1.0f), quad[i].uv});
    }
}
} // namespace Snowglobe