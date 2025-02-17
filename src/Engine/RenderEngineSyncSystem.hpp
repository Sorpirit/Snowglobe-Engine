#pragma once
 
#include "ECS/ISystem.hpp"

namespace Snowglobe::Engine
{
    
class RenderEngineSyncSystem : public Core::ECS::ISystem
{
public:
    RenderEngineSyncSystem() { _updateOrder = Core::ECS::UpdateOrder::PostPhysics; }
    void Update() override;
};

}
