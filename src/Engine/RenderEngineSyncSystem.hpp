#pragma once
 
#include "ECS/ISystem.hpp"

#include "MeshComponent.hpp"

namespace Snowglobe::Engine
{
    
class RenderEngineSyncSystem : public Core::ECS::ISystem
{
public:
    void UpdateLate() override;
};
    

} // namespace Snowglobe::Engine
