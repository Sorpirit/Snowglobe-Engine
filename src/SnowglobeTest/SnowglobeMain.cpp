#include <iostream>
#include <functional>
#include <string>
#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SnowFileSystem.hpp>
#include <SnowEngine.hpp>
#include <Window.hpp>
#include <InputReader.hpp>
#include <RenderSystem.hpp>

#include "CommonTests.hpp"

#include "ECS/Component.hpp"
#include "ECS/Entity.hpp"
#include "ECS/EntityManager.hpp"

#include "TransformComponent.hpp"
#include "Physics2DComponent.hpp"
#include "Collider2DComponent.hpp"
#include "ComponentEditor.hpp"
#include "MeshComponent.hpp"

typedef Snowglobe::SnowCore::ECS::MapEntityData
<
Snowglobe::SnowCore::TransformComponent,
Snowglobe::SnowEngine::Physics2DComponent,
Snowglobe::SnowEngine::Collider2DComponent,
Snowglobe::SnowEngine::MeshComponent,
Snowglobe::SnowEngine::BaseComponentMaterial,
Snowglobe::SnowEngine::DebugComponent

> SampleMapEntityData;



typedef Snowglobe::SnowCore::ECS::EntityManager<SampleMapEntityData> SampleEntityManager;

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main()
{
    Snowglobe::SnowCore::EngineProfile profile = { "Snowglobe", Snowglobe::SnowCore::EngineRenderEngine::OpenGL };
    Snowglobe::Render::WindowParams windowParams = { "Snowglobe", SCR_WIDTH, SCR_HEIGHT, 0, 0, true, false, true, true, false, 0.0, 0.0, "" };

    auto& engine = Snowglobe::SnowEngine::SnowEngine::GetInstance();
    auto& fileSystem = Snowglobe::SnowCore::SnowFileSystem::GetInstance();
    auto manager = std::make_shared<SampleEntityManager>();

    fileSystem.AddMount("C:/Users/danvu/sources/snowglobe/src/RenderOpenGL/Shaders");
    fileSystem.AddMount("C:/Users/danvu/sources/snowglobe/src/SnowglobeTest");
    
    engine.Setup(profile, windowParams, manager);
    
    // Get render proxy
    Snowglobe::Render::RenderSystem* renderSystem = nullptr; 
    if(!engine.QuerySystem<Snowglobe::Render::RenderSystem>(renderSystem))
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
    Assigment1Tests test(engine, fileSystem);
    
    test.Init();
    
    auto updateFunc = [&]()
    {
        if(mainWindow->GetInput().IsKeyPressed(Snowglobe::SnowCore::Key::Escape))
        {
            std::cout << "Escape pressed" << std::endl;
            mainWindow->Close();
        }

        test.Run();
    };

    engine.RegisterUpdateCallback(updateFunc);
    while(mainWindow->IsOpen())
    {
        mainWindow->PollEvents();
        engine.Run();
        mainWindow->Present();
    }
    
    return 0;
}