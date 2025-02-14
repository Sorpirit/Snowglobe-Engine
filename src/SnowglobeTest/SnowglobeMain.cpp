#include <iostream>
#include <functional>
#include <string>
#include <memory>

#include "ECS/Tag.hpp"
#include <FileSystem.hpp>
#include <Engine.hpp>
#include <Window.hpp>
#include <InputReader.hpp>
#include <RenderSystem.hpp>

#include "CommonTests.hpp"

#include "ECS/Entity.hpp"
#include "ECS/EntityManager.hpp"

#include "TransformComponent.hpp"
#include "Physics2DComponent.hpp"
#include "Collider2DComponent.hpp"
#include "LifeLinkComponent.hpp"
#include "LifetimeComponent.hpp"
#include "MeshComponent.hpp"
#include "NEdgeShape2DComponent.hpp"
#include "ShapesShooter2DTest.hpp"
#include "../RenderOpenGL/OpenGLRenderSystem.hpp"

typedef Snowglobe::Core::ECS::MappedTupleEntityData
<
Snowglobe::Core::TransformComponent,
Snowglobe::Engine::Physics2DComponent,
Snowglobe::Engine::Collider2DComponent,
Snowglobe::Engine::MeshComponent,
Snowglobe::Engine::BaseComponentMaterial,
Snowglobe::RenderOpenGL::DirectionalLightComponent,
Snowglobe::RenderOpenGL::PointLightComponent,
Snowglobe::RenderOpenGL::SpotLightComponent,
Snowglobe::Render::NEdgeShape2DComponent,
Snowglobe::Engine::LifetimeComponent,
Snowglobe::PawnInputComponent,
Snowglobe::FadeOutLifetime,
Snowglobe::ExplodeOnDeath,
Snowglobe::DestroyOnCollision,
Snowglobe::MouseControllerComponent,
Snowglobe::RotationAnimationComponent,
Snowglobe::ScoreComponent
> SampleMapEntityData;

typedef Snowglobe::Core::ECS::EntityManager<SampleMapEntityData> SampleEntityManager;

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main()
{
    auto& fileSystem = Snowglobe::Core::FileSystem::GetInstance();
    //Resolve project path
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
    
    fileSystem.AddMount(project_path / "src/RenderOpenGL/Shaders");
    fileSystem.AddMount(project_path / "src/SnowglobeTest/Assets");

    Snowglobe::Core::EngineProfile profile = { "Snowglobe", Snowglobe::Core::EngineRenderEngine::OpenGL };
    Snowglobe::Render::WindowParams windowParams = { "Snowglobe", SCR_WIDTH, SCR_HEIGHT, false, false, true};

    auto& engine = Snowglobe::Engine::Engine::GetInstance();
    auto manager = std::make_shared<SampleEntityManager>();
    engine.Setup(profile, windowParams, manager);
    
    Snowglobe::Render::RenderSystem* renderSystem = nullptr;
    if(!engine.GetSystemManager()->QuerySystem<Snowglobe::Render::RenderSystem>(renderSystem))
    {
        std::cout << "Failed to get render system" << std::endl;
        return -1;
    }

    auto mainWindow = renderSystem->GetMainWindow();

    // BaseShapeFactoryTests test(engine, fileSystem);
    // UITest test(engine, fileSystem);
    // TextureTests test(engine, fileSystem);
    // Phyiscs2DTests test(engine, fileSystem);
    // CameraTests test(engine, fileSystem);
    // Assigment1Tests test(engine, fileSystem, sceneConfig);
    // LightTests test(engine, fileSystem);
    Snowglobe::ShapesShooter2DTest test(engine, fileSystem);
    
    test.Init();

    while(mainWindow->IsOpen())
    {
        mainWindow->PollEvents();
        engine.Run();
        mainWindow->Present();
    }
    
    return 0;
}