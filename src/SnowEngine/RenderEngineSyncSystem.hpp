#pragma once
 
#include "ECS/ISystem.hpp"

#include "MeshComponent.hpp"

namespace Snowglobe::SnowEngine
{
    
class RenderEngineSyncSystem : public SnowCore::ECS::ISystem
{
public:
    RenderEngineSyncSystem(SnowCore::ECS::EntityManagerBase& entityManager) : ISystem(entityManager) {};

    void UpdateLate() override;
};
    

} // namespace Snowglobe::SnowEngine
