#pragma once
#include "Collider2DComponent.hpp"
#include "ECS/Entity.hpp"
#include "Physics2DComponent.hpp"
#include "Serialization/SerializationAPI.hpp"
#include "Serialization/Serializer.hpp"
#include "SpriteRenderComponent.hpp"

namespace Snowglobe::Engine
{

class EngineTypesSerialization
{
public:
    static void RegisterCustomTypesSerialization(Core::Serialization::Serializer& serializer)
    {
        serializer.RegisterCustomProperty<Core::ECS::Entity>([](Core::Serialization::SerializationAPI* api, Core::ECS::Entity& value) {
            std::string name = value.GetName();
            std::string tagName = value.GetTag().GetName();
            api->Property("Name", name);
            api->Property("Tag", tagName);
            std::vector<Core::ECS::Component*> components;
            value.ListAttachedComponents(components);
            api->Property<std::vector<Core::ECS::Component*>, true>("Components", components);
            return true;
        });
    }

    static void RegisterCustomTypes(Core::Serialization::SerializationAPI& api)
    {
        api.RegisterCustomProperty<Collider2DComponent>(
            [](Core::Serialization::SerializationAPI* api, Collider2DComponent& value) {
                uint32_t s = static_cast<uint32_t>(value.ShapeType);
                api->Property("ShapeType", s);
                value.ShapeType = static_cast<CollisionShapeType>(s);
                api->Property("IsTrigger", value.IsTrigger);
                return true;
            });

        api.RegisterCustomProperty<Physics2DComponent>(
            [](Core::Serialization::SerializationAPI* api, Physics2DComponent& value) {
                api->Property("Velocity", value.Velocity);
                api->Property("AngularVelocity", value.AngularVelocity);
                api->Property("Mass", value.Mass);
                api->Property("Drag", value.Drag);
                api->Property("AngularDrag", value.AngularDrag);
                api->Property("GravityScale", value.GravityScale);
                api->Property("Bounciness", value.Bounciness);
                return true;
            });
    }
};

}
