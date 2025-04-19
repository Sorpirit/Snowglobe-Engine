#pragma once
#include "Assets/Asset.hpp"
#include "Assets/AssetManager.hpp"
#include "ECS/Entity.hpp"
#include "FileSystem.hpp"
#include "Serialization/Deserialize.hpp"
#include "Serialization/SerializationAPI.hpp"
#include "Serialization/Serializer.hpp"
#include "TransformComponent.hpp"
#include "glm/glm.hpp"

namespace Snowglobe::Core
{
class CoreTypesSerialization
{
public:
    static void RegisterCustomTypesSerialization(Serialization::Serializer& serializer)
    {
        serializer.RegisterCustomProperty<AssetBase>([](Serialization::SerializationAPI* api, AssetBase& value) {
            std::string tag = value.GetAssetTag();
            api->Property("Tag", tag);
            return true;
        });

        serializer.RegisterCustomProperty<SnowFileHandle>([](Serialization::SerializationAPI* api, SnowFileHandle& value) {
            std::string path = value.GetPath().string();
            api->Property("path", path);
            return true;
        });
    }

    static void RegisterCustomTypesDeserialization(Serialization::Deserializer& deserializer)
    {
        deserializer.RegisterCustomProperty<AssetBase>([](Serialization::SerializationAPI* api, AssetBase& value) {
            std::string tag = "";
            api->Property("Tag", tag);
            DI->Resolve<AssetManager>()->ResolveBaseRef(tag, &value);
            return true;
        });

        deserializer.RegisterCustomProperty<SnowFileHandle>([](Serialization::SerializationAPI* api, SnowFileHandle& value) {
            std::string path = "";
            api->Property("path", path);
            value.Initialize(path);
            return true;
        });
    }

    static void RegisterCustomTypes(Serialization::SerializationAPI& api)
    {
        api.RegisterCustomProperty<TransformComponent>([](Serialization::SerializationAPI* api, TransformComponent& value) {
            api->Property("Position", value.Position);
            api->Property("Rotation", value.Rotation);
            api->Property("Scale", value.Scale);
            return true;
        });

        api.RegisterCustomProperty<glm::vec2>([](Serialization::SerializationAPI* api, glm::vec2& value) {
            api->Property("x", value.x);
            api->Property("y", value.y);
            return true;
        });
        api.RegisterCustomProperty<glm::vec3>([](Serialization::SerializationAPI* api, glm::vec3& value) {
            api->Property("x", value.x);
            api->Property("y", value.y);
            api->Property("z", value.z);
            return true;
        });
        api.RegisterCustomProperty<glm::vec4>([](Serialization::SerializationAPI* api, glm::vec4& value) {
            api->Property("x", value.x);
            api->Property("y", value.y);
            api->Property("z", value.z);
            api->Property("w", value.w);
            return true;
        });
        api.RegisterCustomProperty<glm::ivec2>([](Serialization::SerializationAPI* api, glm::ivec2& value) {
            api->Property("x", value.x);
            api->Property("y", value.y);
            return true;
        });
        api.RegisterCustomProperty<glm::ivec3>([](Serialization::SerializationAPI* api, glm::ivec3& value) {
            api->Property("x", value.x);
            api->Property("y", value.y);
            api->Property("z", value.z);
            return true;
        });
        api.RegisterCustomProperty<glm::ivec4>([](Serialization::SerializationAPI* api, glm::ivec4& value) {
            api->Property("x", value.x);
            api->Property("y", value.y);
            api->Property("z", value.z);
            api->Property("w", value.w);
            return true;
        });
    }
};
}