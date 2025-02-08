#pragma once

#include "ECS/Component.hpp"

namespace Snowglobe::Engine
{
    
class LifetimeComponent : public Core::ECS::Component
{
public:
    LifetimeComponent() = default;
    LifetimeComponent(float lifeTime) : Lifetime(lifeTime), LifetimeInitial(lifeTime) {}
    
    float Lifetime = 0.0f;
    float LifetimeInitial = 0.0f;
};

}
