#pragma once
 
#include "ECS/ISystem.hpp"

namespace Snowglobe::Engine
{
    
class LifeLinkSystem : public Core::ECS::ISystem
{
public:
    LifeLinkSystem() { _updateOrder = Core::ECS::UpdateOrder::PostRender; }
    void Update();
};

}
