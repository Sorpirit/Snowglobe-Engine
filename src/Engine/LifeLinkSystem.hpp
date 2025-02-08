#pragma once
 
#include "ECS/ISystem.hpp"

namespace Snowglobe::Engine
{
    
class LifeLinkSystem : public Core::ECS::ISystem
{
public:
    void UpdateLate() override;
};

}
