#include "ModelLoading.hpp"

#include "AssimpImporter.hpp"
#include "MeshComponent.hpp"
#include "TransformComponent.hpp"

namespace Snowglobe {

void ModelLoading::Init()
{
    RuntimeTest::Init();
    auto manager = _engine->GetEntityManager();

    Engine::AssimpImporter importer;
    auto meshList = importer.LoadModel(Core::SnowFileHandle("Models/backpack/backpack.obj"));
    auto backpack = manager->CreateEntity();
    backpack->SetName("Backpack");
    backpack->AddComponent<Core::TransformComponent>(glm::vec3(0, -1, 0), glm::vec3(0.0f), glm::vec3(0.5f));
    backpack->AddComponent<Engine::ModelComponent>(meshList);
}

void ModelLoading::Run() {}

} // Snowglobe