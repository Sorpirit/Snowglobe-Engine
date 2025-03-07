#pragma once
#include "glm/glm.hpp"
#include <cstdint>
#include <iostream>
#include <string>
#include <typeindex>
#include <typeinfo>

namespace Snowglobe::Core::Serialization
{
class SerializationAPI;
}

template <typename T> void CustomProperty(Snowglobe::Core::Serialization::SerializationAPI* api, T* value, uint32_t metaFlags);
template <typename T> void CustomPropertySerialization(Snowglobe::Core::Serialization::SerializationAPI* api, T* value, uint32_t metaFlags);
template <typename T> void CustomPropertyDeserialization(Snowglobe::Core::Serialization::SerializationAPI* api, T* value, uint32_t metaFlags);

namespace Snowglobe::Core::Serialization
{
template <typename CComponent, typename SSerializer>
concept CustomPropertySerializer = requires(SSerializer serializer, CComponent* component) {
    {
        serializer.Property(std::declval<SerializationAPI*>(), component, std::declval<uint32_t>())
    } -> std::convertible_to<void>;
};

class SerializationAPI
{
  public:
    template <typename T, bool Dynamic = false> void Property(T& value, uint32_t metaFlags = 0)
    {
        if (Dynamic)
        {
            std::string type = typeid(value).name();
            BaseProperty("Type", type);
            if (!TryPushObject("Data"))
                return;

            PropertyInternal(type, &value, metaFlags);
            PopObject();
        }
        else
        {
            // const std::type_index& baseType = typeid(T);
            // const std::type_index& dynamicType = typeid(value);
            // assert(baseType == dynamicType && "Property type mismatch");
            PropertyInternal(typeid(T).name(), static_cast<void*>(&value), metaFlags);
        }
    }

    template <typename T, bool Dynamic = false> void Property(const std::string& name, T& value, uint32_t metaFlags = 0)
    {
        if (!TryPushObject(name))
            return;

        Property<T, Dynamic>(value, metaFlags);
        PopObject();
    }

    template <typename T, bool Dynamic = false> void Property(const std::string& name, T& value, T& defaultValue, uint32_t metaFlags = 0)
    {
        if (!TryPushObject(name))
        {
            value = defaultValue; //think about it
            return;
        }

        Property<T, Dynamic>(value, metaFlags);
        PopObject();
    }

    template <typename T, bool Dynamic = false> void Property(const std::string& name, std::vector<T*>& value, uint32_t metaFlags = 0)
    {
        uint32_t size = static_cast<uint32_t>(value.size());
        if (!TryPushArray(name, size))
            return;

        for (uint32_t i = 0; i < size; i++)
        {
            PushArrayItemAt(i);
            Property<T, Dynamic>(*value[i], metaFlags);
            PopObject();
        }

        PopObject();
    }

    template <typename T, bool Dynamic = false>
    void Property(const std::string& name, std::vector<std::shared_ptr<T>>& value, uint32_t metaFlags = 0)
    {
        uint32_t size = static_cast<uint32_t>(value.size());
        if (!TryPushArray(name, size))
            return;

        for (uint32_t i = 0; i < size; i++)
        {
            PushArrayItemAt(i);
            Property<T, Dynamic>(*value[i], metaFlags);
            PopObject();
        }

        PopObject();
    }

    virtual void BaseProperty(const std::string& name, int& value, uint32_t metaFlags = 0) = 0;
    virtual void BaseProperty(const std::string& name, float& value, uint32_t metaFlags = 0) = 0;
    virtual void BaseProperty(const std::string& name, uint32_t& value, uint32_t metaFlags = 0) = 0;
    virtual void BaseProperty(const std::string& name, std::string& value, uint32_t metaFlags = 0) = 0;
    virtual void BaseProperty(const std::string& name, bool& value, uint32_t metaFlags = 0) = 0;

    template <typename CComponent, CustomPropertySerializer<CComponent> T> void RegisterCustomProperty(T* ptr)
    {
        _properties[typeid(CComponent).name()] = [&](SerializationAPI* api, void* value, uint32_t metaFlags) {
            ptr->Property(api, static_cast<CComponent*>(value), metaFlags);
        };
    }

    template <typename CComponent>
    void RegisterCustomProperty(void func(SerializationAPI* api, CComponent* value, uint32_t metaFlags))
    {
        _properties[typeid(CComponent).name()] = [=](SerializationAPI* api, void* value, uint32_t metaFlags) {
            func(api, static_cast<CComponent*>(value), metaFlags);
        };
    }

    template <typename CComponent>
    void RegisterCustomProperty()
    {
        _properties[typeid(CComponent).name()] = [=](SerializationAPI* api, void* value, uint32_t metaFlags) {
            CustomProperty<CComponent>(api, static_cast<CComponent*>(value), metaFlags);
        };
    }

    template <typename CComponent>
    void RegisterCustomPropertySerialization()
    {
        _properties[typeid(CComponent).name()] = [=](SerializationAPI* api, void* value, uint32_t metaFlags) {
            CustomPropertySerialization<CComponent>(api, static_cast<CComponent*>(value), metaFlags);
        };
    }

    template <typename CComponent>
    void RegisterCustomPropertyDeserialization()
    {
        _properties[typeid(CComponent).name()] = [=](SerializationAPI* api, void* value, uint32_t metaFlags) {
            CustomPropertyDeserialization<CComponent>(api, static_cast<CComponent*>(value), metaFlags);
        };
    }

  protected:
    virtual bool TryPushObject(const std::string& name) = 0;
    virtual bool TryPushArray(const std::string& name, uint32_t& size) = 0;
    virtual void PushArrayItemAt(uint32_t index) = 0;
    virtual void PopObject() = 0;

    void PropertyInternal(const std::string& type, void* ptr, uint32_t metaFlags = 0)
    {
        auto it = _properties.find(type);
        if (it == _properties.end())
        {
            assert(0 && "Unknown type");
            return;
        }

        it->second(this, ptr, metaFlags);
    }

  private:
    std::unordered_map<std::string, std::function<void(SerializationAPI*, void*, uint32_t)>> _properties;
};

} // namespace Snowglobe::Core::Serialization

template <>
inline void CustomProperty<int>(Snowglobe::Core::Serialization::SerializationAPI* api, int* value, uint32_t metaFlags)
{
    api->BaseProperty("", *value, metaFlags);
}

template <>
inline void CustomProperty<float>(Snowglobe::Core::Serialization::SerializationAPI* api, float* value, uint32_t metaFlags)
{
    api->BaseProperty("", *value, metaFlags);
}

template <>
inline void CustomProperty<uint32_t>(Snowglobe::Core::Serialization::SerializationAPI* api, uint32_t* value, uint32_t metaFlags)
{
    api->BaseProperty("", *value, metaFlags);
}

template <>
inline void CustomProperty<std::string>(Snowglobe::Core::Serialization::SerializationAPI* api, std::string* value, uint32_t metaFlags)
{
    api->BaseProperty("", *value, metaFlags);
}

template <>
inline void CustomProperty<glm::vec2>(Snowglobe::Core::Serialization::SerializationAPI* api, glm::vec2* value, uint32_t metaFlags)
{
    api->BaseProperty("x", value->x, metaFlags);
    api->BaseProperty("y", value->y, metaFlags);
}

template <>
inline void CustomProperty<glm::vec3>(Snowglobe::Core::Serialization::SerializationAPI* api, glm::vec3* value, uint32_t metaFlags)
{
    api->BaseProperty("x", value->x, metaFlags);
    api->BaseProperty("y", value->y, metaFlags);
    api->BaseProperty("z", value->z, metaFlags);
}

template <>
inline void CustomProperty<glm::vec4>(Snowglobe::Core::Serialization::SerializationAPI* api, glm::vec4* value, uint32_t metaFlags)
{
    api->BaseProperty("x", value->x, metaFlags);
    api->BaseProperty("y", value->y, metaFlags);
    api->BaseProperty("z", value->z, metaFlags);
    api->BaseProperty("w", value->w, metaFlags);
}

template <>
inline void CustomProperty<glm::ivec2>(Snowglobe::Core::Serialization::SerializationAPI* api, glm::ivec2* value, uint32_t metaFlags)
{
    api->BaseProperty("x", value->x, metaFlags);
    api->BaseProperty("y", value->y, metaFlags);
}

template <>
inline void CustomProperty<glm::ivec3>(Snowglobe::Core::Serialization::SerializationAPI* api, glm::ivec3* value, uint32_t metaFlags)
{
    api->BaseProperty("x", value->x, metaFlags);
    api->BaseProperty("y", value->y, metaFlags);
    api->BaseProperty("z", value->z, metaFlags);
}

template <>
inline void CustomProperty<glm::ivec4>(Snowglobe::Core::Serialization::SerializationAPI* api, glm::ivec4* value, uint32_t metaFlags)
{
    api->BaseProperty("x", value->x, metaFlags);
    api->BaseProperty("y", value->y, metaFlags);
    api->BaseProperty("z", value->z, metaFlags);
    api->BaseProperty("w", value->w, metaFlags);
}