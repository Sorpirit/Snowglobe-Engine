#pragma once
 
#include "ECS/ISystem.hpp"

namespace Snowglobe::Engine
{
    
class RenderEngineSyncSystem : public Core::ECS::ISystem
{
public:
    void UpdateLate() override;
};

}
