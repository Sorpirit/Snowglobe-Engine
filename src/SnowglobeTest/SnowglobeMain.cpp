#include <iostream>
#include <functional>
#include <string>
#include <memory>

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

    Snowglobe::SnowEngine::SnowEngine& engine = Snowglobe::SnowEngine::SnowEngine::GetInstance();
    auto fileSystem = Snowglobe::SnowCore::SnowFileSystem::GetInstance();

    fileSystem->AddMount("C:/Users/danvu/sources/snowglobe/src/RenderOpenGL/Shaders");
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
    auto uiSystem = renderSystem->GetUISystem();

    Snowglobe::Render::BasicShapeFactory shapeFactory(renderSystem);

    auto material = renderSystem->CreateMaterialInstance<Snowglobe::Render::BasicShapeMaterial>();

    //Create a circle
    Snowglobe::SnowEngine::SnowEntity& circle = engine.CreateEntity();
    auto circleMesh = shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Disk, glm::vec3(0.0f), glm::vec3(1.0f));
    circleMesh->SetMaterial(material.GetMaterialBase());
    auto circleMeshComponent = std::make_shared<Snowglobe::SnowEngine::MeshComponent>(*circleMesh);
    circle.AddComponent(circleMeshComponent.get());
    auto circlePhysics = physicsEngine->AttachPhysicsComponent(circle);
    physicsEngine->AttachCollisionComponent(circle, Snowglobe::SnowEngine::CollisionShapeType::Circle);
    circle.SetPosition(glm::vec3(-3.0f, 0.0f, 0.0f));
    circlePhysics->SetVelocity(glm::vec2(2.0f, 0.5f));

    //Create a plane
    Snowglobe::SnowEngine::SnowEntity& plane = engine.CreateEntity();
    auto planeMesh = shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
    planeMesh->SetMaterial(material.GetMaterialBase());
    auto planeMeshComponent = std::make_shared<Snowglobe::SnowEngine::MeshComponent>(*planeMesh);
    plane.AddComponent(planeMeshComponent.get());
    auto planePhysics = physicsEngine->AttachPhysicsComponent(plane);
    physicsEngine->AttachCollisionComponent(plane, Snowglobe::SnowEngine::CollisionShapeType::AABB);
    planePhysics->SetVelocity(glm::vec2(-1.0f, 0.5f));
    
    
    int comboItem = 0;
    std::vector<std::string> items = { "Item 1", "Item 2", "Item 3" };

    bool showDemoWindow = false;

    const size_t nameSize = 128; 
    char triangleName[nameSize] = "Triangle";
    
    while(mainWindow->IsOpen())
    {
        mainWindow->PollEvents();
        engine.StartFrame();

        if(mainWindow->GetInput().IsKeyPressed(Snowglobe::SnowCore::Key::Escape))
        {
            std::cout << "Escape pressed" << std::endl;
            mainWindow->Close();
        }

        double timeValue = Snowglobe::SnowCore::EngineTime::GetTimeSinceStart();
        
        {
            auto porp = uiSystem->OpenUIPanel("Properties");

            uiSystem->Text("Hello, world!");
            //uiSystem->Checkbox("Checkbox", &showDemoWindow);
            uiSystem->Combo("Combo", &comboItem, items);
            uiSystem->Color("Color", &material.Properties()->color);
            uiSystem->InputText("Name", triangleName, nameSize);
            //uiSystem->Slider("Mesh position", &meshPosition, -5.0f, 5.0f);
        }

        uiSystem->AddWorldText(glm::vec3(0.0f), std::string(triangleName), glm::vec3(1.0f, 1.0f, 0.0f), Snowglobe::Render::Alignment::HorizontalCenter | Snowglobe::Render::Alignment::VerticalCenter);
        
        engine.Update();
        mainWindow->Present();
    }
    
    
    

    return 0;
}