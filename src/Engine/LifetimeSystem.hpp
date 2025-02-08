#pragma once
 
#include "ECS/ISystem.hpp"

namespace Snowglobe::Engine
{
    
class LifetimeSystem : public Core::ECS::ISystem
{
public:
    void Update() override;
};

}
