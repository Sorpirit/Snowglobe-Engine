#include "ECSTest.hpp"

#include "ECS/LifetimeSystem.hpp"
#include "ECS/SystemManager.hpp"
#include "ECS/UpdateOrder.hpp"

#include <iostream>

using namespace Snowglobe::Core::ECS::ECSTest;

void Update(
    const std::shared_ptr<Snowglobe::Core::ECS::EntityManagerBase>& entityManager,
    const std::shared_ptr<Snowglobe::Core::ECS::SystemManager>& systems,
    const std::shared_ptr<Snowglobe::Core::ECS::LifetimeSystem>& lifetimeSystem)
{
    static uint32_t frameN = 0;

    entityManager->Update();
    systems->Update();

    frameN++;
}

int main()
{
    auto nativeEntityManager = std::make_shared<ECSTestEntityManager>();
    std::shared_ptr<Snowglobe::Core::ECS::EntityManagerBase> entityManager = nativeEntityManager;
    auto systemManager = std::make_shared<Snowglobe::Core::ECS::SystemManager>(entityManager);
    auto lifetimeSystem = std::make_shared<Snowglobe::Core::ECS::LifetimeSystem>();

    systemManager->TryAddSystem<RenderSystem>();
    systemManager->TryAddSystem<Physics2DSystem>();

    systemManager->TryAddSystem([](const std::shared_ptr<Snowglobe::Core::ECS::EntityManagerBase>& enityManager) {
            std::cout << "DSystem update" << '\n';
    });

    Update(entityManager, systemManager, lifetimeSystem);
    Update(entityManager, systemManager, lifetimeSystem);
    Update(entityManager, systemManager, lifetimeSystem);

    //todo
    //Systems
    // Register/Unregister
    // Permanents Test: Permanent systems cant be deactivated or Removed
    // Inactive systems should not get update
    // Update order
}