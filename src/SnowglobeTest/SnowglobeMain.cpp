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

#include "BasicShapeFactory.hpp"

#include "EngineTime.hpp"
#include "UISystem.hpp"

#include "PhysicsEngine2DSystem.hpp"
#include "MeshComponent.hpp"

#include "CommonVertexLayouts.hpp"
#include "MaterialsData/TextureColorMaterialData.hpp"

#include "CommonTests.hpp"

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main()
{
    Snowglobe::SnowCore::EngineProfile profile = { "Snowglobe", Snowglobe::SnowCore::EngineRenderEngine::OpenGL };
    Snowglobe::Render::WindowParams windowParams = { "Snowglobe", SCR_WIDTH, SCR_HEIGHT, 0, 0, true, false, true, true, false, 0.0, 0.0, "" };

    Snowglobe::SnowEngine::SnowEngine& engine = Snowglobe::SnowEngine::SnowEngine::GetInstance();
    auto fileSystem = Snowglobe::SnowCore::SnowFileSystem::GetInstance();

    fileSystem->AddMount("C:/Users/danvu/sources/snowglobe/src/RenderOpenGL/Shaders");
    fileSystem->AddMount("C:/Users/danvu/sources/snowglobe/src/SnowglobeTest");
    
    engine.Setup(profile, windowParams);
    
    // Get render proxy
    Snowglobe::Render::RenderSystem* renderSystem = nullptr; 
    if(!engine.QuerySystem<Snowglobe::Render::RenderSystem>(renderSystem))
    {
        std::cout << "Failed to get render system" << std::endl;
        return -1;
    }

    Snowglobe::SnowEngine::PhysicsEngine2DSystem* physicsEngine = nullptr;
    if(!engine.QuerySystem(physicsEngine))
    {
        std::cout << "Failed to get render system" << std::endl;
        return -1;
    }

    auto mainWindow = renderSystem->GetMainWindow();

    // BaseShapeFactoryTests test(engine, *fileSystem);
    // UITest test(engine, *fileSystem);
    // TextureTests test(engine, *fileSystem);
    // Phyiscs2DTests test(engine, *fileSystem);
    CameraTests test(engine, *fileSystem);
    
    test.Init();
    
    while(mainWindow->IsOpen())
    {
        mainWindow->PollEvents();
        engine.StartFrame();

        if(mainWindow->GetInput().IsKeyPressed(Snowglobe::SnowCore::Key::Escape))
        {
            std::cout << "Escape pressed" << std::endl;
            mainWindow->Close();
        }

        test.Run();
        
        engine.Update();
        mainWindow->Present();
    }
    
    
    

    return 0;
}