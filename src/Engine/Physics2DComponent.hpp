#pragma once

#include "Serialization/SerializationAPI.hpp"

#include <glm/glm.hpp>

#include "ECS/Component.hpp"

namespace Snowglobe::Engine
{

class Physics2DComponent : public Core::ECS::Component
{
public:
    Physics2DComponent() = default;
    Physics2DComponent(const glm::vec2& velocity, float angularVelocity = 0.0f, float mass = 1.0f, float drag = 0.0f, float angularDrag = 0.0f, float gravityScale = 1.0f) :
        Velocity(velocity), AngularVelocity(angularVelocity), Mass(mass), Drag(drag), AngularDrag(angularDrag), GravityScale(gravityScale) {}
    
    glm::vec2 Velocity = glm::vec2(0.0f);
    float AngularVelocity = 0.0f;
    float Mass = 1.0f;
    float Drag = 0.0f;
    float AngularDrag = 0.0f;
    float GravityScale = 1.0f;
    float Bouncines = 1.0f;
};
}

template <>
inline void CustomProp<Snowglobe::Engine::Physics2DComponent>(Snowglobe::Core::Serialization::SerializationAPI* api,
                                                               Snowglobe::Engine::Physics2DComponent* value, uint32_t metaFlags)
{
    api->Property("Velocity", value->Velocity);
    api->BaseProperty("AngularVelocity", value->AngularVelocity);
    api->BaseProperty("Mass", value->Mass);
    api->BaseProperty("AngularDrag", value->AngularDrag);
    api->BaseProperty("GravityScale", value->GravityScale);
    api->BaseProperty("Bouncines", value->Bouncines);
}