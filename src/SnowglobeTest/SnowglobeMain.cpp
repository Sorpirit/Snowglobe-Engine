#include <iostream>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SnowEngine.hpp>
#include <Window.hpp>
#include <RenderSystem.hpp>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // TODO:
    // Init Engine core
    // Setup basic parameters
    // Specify list of reqiered modules
    Snowglobe::SnowCore::EngineProfile profile = { "Snowglobe", Snowglobe::SnowCore::EngineRenderEngine::OpenGL };
    Snowglobe::Render::WindowParams windowParams = { "Snowglobe", SCR_WIDTH, SCR_HEIGHT, 0, 0, true, false, true, true, false, 0.0, 0.0, "" };

    auto engine = Snowglobe::SnowEngine::SnowEngine::GetInstance();
    
    engine->Setup(profile);
    
    // Get render proxy
    Snowglobe::Render::RenderSystem* renderSystem = nullptr; 
    if(!engine->QuerySystem<Snowglobe::Render::RenderSystem>(renderSystem))
    {
        std::cout << "Failed to get render system" << std::endl;
        return -1;
    }

    renderSystem->InitializeWindow(windowParams);

    auto mainWindow = renderSystem->GetMainWindow();

    // Add shapes
    // Run update while running
    while(mainWindow->IsOpen())
    {
        engine->Update();
    }
    
    
    

    return 0;
}