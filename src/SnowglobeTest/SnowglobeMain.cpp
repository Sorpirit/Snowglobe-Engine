#include <iostream>
#include <functional>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SnowFileSystem.hpp>
#include <SnowEngine.hpp>
#include <Window.hpp>
#include <InputReader.hpp>
#include <RenderSystem.hpp>

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main()
{
    // TODO:
    // Init Engine core
    // Setup basic parameters
    // Specify list of reqiered modules
    Snowglobe::SnowCore::EngineProfile profile = { "Snowglobe", Snowglobe::SnowCore::EngineRenderEngine::OpenGL };
    Snowglobe::Render::WindowParams windowParams = { "Snowglobe", SCR_WIDTH, SCR_HEIGHT, 0, 0, true, false, true, true, false, 0.0, 0.0, "" };

    auto engine = Snowglobe::SnowEngine::SnowEngine::GetInstance();
    auto fileSystem = Snowglobe::SnowCore::SnowFileSystem::GetInstance();

    fileSystem->AddMount("C:/Users/danvu/sources/snowglobe/src/RenderOpenGL/Shaders");
    engine->Setup(profile, windowParams);
    
    // Get render proxy
    Snowglobe::Render::RenderSystem* renderSystem = nullptr; 
    if(!engine->QuerySystem<Snowglobe::Render::RenderSystem>(renderSystem))
    {
        std::cout << "Failed to get render system" << std::endl;
        return -1;
    }

    auto mainWindow = renderSystem->GetMainWindow();

    while(mainWindow->IsOpen())
    {
        mainWindow->PollEvents();

        if(mainWindow->GetInput().IsKeyPressed(Snowglobe::SnowCore::Key::Escape))
        {
            std::cout << "Escape pressed" << std::endl;
            mainWindow->Close();
        }

        engine->Update();
        mainWindow->Present();
    }
    
    
    

    return 0;
}