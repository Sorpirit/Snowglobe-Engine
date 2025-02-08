#pragma once

#include <memory>
#include <vector>

#include "ECS/Component.hpp"
#include "ECS/Entity.hpp"

namespace Snowglobe::Core
{
    
class LifeLinkComponent : public ECS::Component
{
public:
    LifeLinkComponent() = default;

    std::vector<std::weak_ptr<ECS::Entity>> LinkedEntities;
};

}
