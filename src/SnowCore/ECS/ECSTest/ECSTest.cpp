#include "ECSTest.hpp"

#include <iostream>
#include <tuple>

using namespace Snowglobe::SnowCore::ECS::ECSTest;

int main()
{
    ECSTestEntityManager nativeEntityManager;
    Snowglobe::SnowCore::ECS::EntityManagerBase& entityManager = nativeEntityManager;

    RenderSystem renderSystem(entityManager);
    Physics2DSystem physicsEngine(entityManager);

    auto baseEnt1 = entityManager.CreateEntity();
    auto baseEnt2 = entityManager.CreateEntity();

    auto taggedEnt1 = entityManager.CreateEntity(1);

    std::string b1 = "BaseEnt1";
    baseEnt1->AddComponent<DebugComponent>(b1);
    baseEnt2->AddComponent<DebugComponent>("BaseEnt2");
    taggedEnt1->AddComponent<DebugComponent>("TaggedEnt1");

    baseEnt1->AddComponent<TestComponent>(1, 2.0f, "Test1");
    
    baseEnt1->AddComponent<TransformComponent>();
    
    baseEnt2->AddComponent<TransformComponent>();
    TransformComponent* transform2 = nullptr;
    if (baseEnt2->QueryComponent(transform2))
    {
        transform2->Position = glm::vec3(3.0f, 2.0f, 1.0f);
        transform2->Rotation = glm::vec3(45.0f, 0.0f, 45.0f);
        transform2->Scale = glm::vec3(1.0f, 2.0f, 3.0f);
    }
    
    baseEnt1->AddComponent<Physics2DComponent>(glm::vec2(1.0f, 2.0f), 1.0f);
    baseEnt2->AddComponent<Physics2DComponent>(glm::vec2(2.0f, 1.0f), 2.0f);
    
    baseEnt1->AddComponent<SpriteComponent>(&renderSystem, "Sprite1");
    baseEnt2->AddComponent<SpriteComponent>(&renderSystem, "Sprite2");
    
    if (baseEnt1->HasComponent<Physics2DComponent>() && !taggedEnt1->HasComponent<Physics2DComponent>())
    {
        std::cout << "baseEnt1 has Physics2DComponent and taggedEnt1 does not. HasComponent works" << std::endl;
    }
    else
    {
        std::cout << "baseEnt1 has Physics2DComponent and taggedEnt1 does not. HasComponent failed" << std::endl;
    }
    
    //Game loop
    {
        //Frame 1
        entityManager.Update();
        renderSystem.Update();
        physicsEngine.Update();
        
        baseEnt1->SetActive(false);
    
        //Frame 2
        entityManager.Update();
        renderSystem.Update();
        physicsEngine.Update();
    
        baseEnt1->SetActive(true);
    
        //Frame 3
        entityManager.Update();
        renderSystem.Update();
        physicsEngine.Update();
    
        baseEnt2->RemoveComponent<Physics2DComponent>();
    
        //Frame 4
        entityManager.Update();
        // renderSystem.Update(); //skip
        physicsEngine.Update();
    
        baseEnt2->RemoveComponent<SpriteComponent>();
    
        //Frame 5
        entityManager.Update();
        renderSystem.Update(); 
        // physicsEngine.Update(); //skip
    
        baseEnt1->Destroy();
    
        //Frame 6
        entityManager.Update();
        renderSystem.Update();
        physicsEngine.Update();
    }
    
}