#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "ECS/Tag.hpp"
#include <Engine.hpp>
#include <FileSystem.hpp>
#include <InputReader.hpp>
#include <RenderSystem.hpp>
#include <Window.hpp>

#include "CommonTests.hpp"

#include "ECS/Entity.hpp"
#include "ECS/EntityManager.hpp"

#include "../RenderOpenGL/OpenGLRenderSystem.hpp"
#include "Collider2DComponent.hpp"
#include "DependencyManager.hpp"
#include "LifeLinkComponent.hpp"
#include "LifetimeComponent.hpp"
#include "MeshComponent.hpp"
#include "NEdgeShape2DComponent.hpp"
#include "Physics2DComponent.hpp"
#include "ShapesShooter2DTest.hpp"
#include "TransformComponent.hpp"

typedef Snowglobe::Core::ECS::MappedTupleEntityData<
    Snowglobe::Core::TransformComponent, Snowglobe::Engine::Physics2DComponent, Snowglobe::Engine::Collider2DComponent,
    Snowglobe::Engine::MeshComponent, Snowglobe::Engine::ModelComponent, Snowglobe::Engine::BaseComponentMaterial,
    Snowglobe::RenderOpenGL::DirectionalLightComponent, Snowglobe::RenderOpenGL::PointLightComponent,
    Snowglobe::RenderOpenGL::SpotLightComponent, Snowglobe::Render::NEdgeShape2DComponent,
    Snowglobe::Engine::LifetimeComponent, Snowglobe::PawnInputComponent, Snowglobe::FadeOutLifetime,
    Snowglobe::ExplodeOnDeath, Snowglobe::DestroyOnCollision, Snowglobe::MouseControllerComponent,
    Snowglobe::RotationAnimationComponent, Snowglobe::ScoreComponent>
    SampleMapEntityData;

typedef Snowglobe::Core::ECS::EntityManager<SampleMapEntityData> SampleEntityManager;

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

void SetupFileSystem()
{
    auto fileSystem = DI->RegisterSingle<Snowglobe::Core::FileSystem>();

    // Resolve project path
    auto project_path = std::filesystem::current_path();
    bool foundProjectPath = false;
    while (project_path.has_parent_path() && project_path != project_path.root_path())
    {
        if (project_path.has_filename() && project_path.filename() == "snowglobe")
        {
            foundProjectPath = true;
            break;
        }
        project_path = project_path.parent_path();
    }

    // If we didn't find the project path, just use the current path
    if (!foundProjectPath)
    {
        project_path = std::filesystem::current_path();
    }

    fileSystem->AddMount(project_path / "src/RenderOpenGL/Shaders");
    fileSystem->AddMount(project_path / "src/SnowglobeTest/Assets");
}

void RegisterTests()
{
    RuntimeTest::RegisterTest<Snowglobe::ShapesShooter2DTest>();
    RuntimeTest::RegisterTest<LightTests>();
    RuntimeTest::RegisterTest<Phyiscs2DTests>();
}

int main(int argc, char* argv[])
{
    SetupFileSystem();

    Snowglobe::Core::EngineProfile profile = {"Snowglobe", Snowglobe::Core::EngineRenderEngine::OpenGL};
    Snowglobe::Render::WindowParams windowParams = {"Snowglobe", SCR_WIDTH, SCR_HEIGHT, false, false, true};

    auto engine = DI->RegisterSingle<Snowglobe::Engine::Engine>();
    auto manager = std::make_shared<SampleEntityManager>();
    engine->Setup(profile, windowParams, manager);

    Snowglobe::Render::RenderSystem* renderSystem = nullptr;
    if (!engine->GetSystemManager()->QuerySystem<Snowglobe::Render::RenderSystem>(renderSystem))
    {
        std::cout << "Failed to get render system" << std::endl;
        return -1;
    }

    RegisterTests();
    std::string testName = "SpaceShooter2DTest";
    for (int i = 0; i < argc; ++i)
    {
        if (std::string(argv[i]) == "--test" && (i + 1) < argc)
        {
            testName = std::string(argv[i + 1]);
        }
    }

    auto& test = RuntimeTest::GetTest(testName);
    test.Init();

    auto mainWindow = renderSystem->GetMainWindow();
    while (mainWindow->IsOpen())
    {
        mainWindow->PollEvents();
        engine->Run();
        mainWindow->Present();
    }

    return 0;
}