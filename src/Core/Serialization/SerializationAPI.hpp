#pragma once
#include "glm/glm.hpp"
#include <cstdint>
#include <string>
#include <iostream>
#include <typeinfo>
#include <typeindex>

namespace Snowglobe::Core::Serialization
{

class SerializationAPI;
template <typename CComponent, typename SSerializer>
concept CustomProperty = requires(SSerializer serializer, CComponent* component) {
    {
        serializer.Property(std::declval<SerializationAPI*>(), component, std::declval<uint32_t>())
    } -> std::convertible_to<void>;
};

class SerializationAPI
{
  public:
    template <typename T> void Property(T& value, uint32_t metaFlags = 0)
    {
        const std::type_index& baseType = typeid(T);
        const std::type_index& dynamicType = typeid(value);
        std::cout << baseType.name() << std::endl;
        std::cout << dynamicType.name() << std::endl;
        if (baseType == dynamicType)
        {
            PropertyInternal(typeid(T).name(), static_cast<void*>(&value), metaFlags);
        }
        else
        {
            std::string type = typeid(value).name();
            BaseProperty("Type", type);
            if (!TryPushObject("Data"))
                return;

            PropertyInternal(type, &value, metaFlags);
            PopObject();
        }
    }

    void Property(const std::string& name, auto& value, uint32_t metaFlags = 0)
    {
        if (!TryPushObject(name))
            return;

        Property(value, metaFlags);
        PopObject();
    }

    template <typename T> void Property(const std::string& name, std::vector<T>& value, uint32_t metaFlags = 0)
    {
        uint32_t size = static_cast<uint32_t>(value.size());
        if (!TryPushArray(name, size))
            return;

        for (uint32_t i = 0; i < size; i++)
        {
            PushArrayItemAt(i);
            Property(value[i], metaFlags);
            PopObject();
        }

        PopObject();
    }

    template <typename T> void Property(const std::string& name, std::vector<T*>& value, uint32_t metaFlags = 0)
    {
        uint32_t size = static_cast<uint32_t>(value.size());
        if (!TryPushArray(name, size))
            return;

        for (uint32_t i = 0; i < size; i++)
        {
            PushArrayItemAt(i);
            Property(*value[i], metaFlags);
            PopObject();
        }

        PopObject();
    }

    template <typename T>
    void Property(const std::string& name, std::vector<std::shared_ptr<T>>& value, uint32_t metaFlags = 0)
    {
        uint32_t size = static_cast<uint32_t>(value.size());
        if (!TryPushArray(name, size))
            return;

        for (uint32_t i = 0; i < size; i++)
        {
            PushArrayItemAt(i);
            Property(*value[i], metaFlags);
            PopObject();
        }

        PopObject();
    }

    virtual void BaseProperty(const std::string& name, int& value, uint32_t metaFlags = 0) = 0;
    virtual void BaseProperty(const std::string& name, float& value, uint32_t metaFlags = 0) = 0;
    virtual void BaseProperty(const std::string& name, uint32_t& value, uint32_t metaFlags = 0) = 0;
    virtual void BaseProperty(const std::string& name, std::string& value, uint32_t metaFlags = 0) = 0;
    virtual void BaseProperty(const std::string& name, bool& value, uint32_t metaFlags = 0) = 0;

    template <typename CComponent, CustomProperty<CComponent> T> void RegisterCustomProperty(T* ptr)
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

template <typename T> void CustomProp(Snowglobe::Core::Serialization::SerializationAPI* api, T* value, uint32_t metaFlags);

template <>
inline void CustomProp<int>(Snowglobe::Core::Serialization::SerializationAPI* api, int* value, uint32_t metaFlags)
{
    api->BaseProperty("", *value, metaFlags);
}

template <>
inline void CustomProp<float>(Snowglobe::Core::Serialization::SerializationAPI* api, float* value, uint32_t metaFlags)
{
    api->BaseProperty("", *value, metaFlags);
}

template <>
inline void CustomProp<uint32_t>(Snowglobe::Core::Serialization::SerializationAPI* api, uint32_t* value, uint32_t metaFlags)
{
    api->BaseProperty("", *value, metaFlags);
}

template <>
inline void CustomProp<std::string>(Snowglobe::Core::Serialization::SerializationAPI* api, std::string* value, uint32_t metaFlags)
{
    api->BaseProperty("", *value, metaFlags);
}

template <>
inline void CustomProp<glm::vec2>(Snowglobe::Core::Serialization::SerializationAPI* api, glm::vec2* value, uint32_t metaFlags)
{
    api->BaseProperty("x", value->x, metaFlags);
    api->BaseProperty("y", value->y, metaFlags);
}

template <>
inline void CustomProp<glm::vec3>(Snowglobe::Core::Serialization::SerializationAPI* api, glm::vec3* value, uint32_t metaFlags)
{
    api->BaseProperty("x", value->x, metaFlags);
    api->BaseProperty("y", value->y, metaFlags);
    api->BaseProperty("z", value->z, metaFlags);
}

template <>
inline void CustomProp<glm::vec4>(Snowglobe::Core::Serialization::SerializationAPI* api, glm::vec4* value, uint32_t metaFlags)
{
    api->BaseProperty("x", value->x, metaFlags);
    api->BaseProperty("y", value->y, metaFlags);
    api->BaseProperty("z", value->z, metaFlags);
    api->BaseProperty("w", value->w, metaFlags);
}

template <>
inline void CustomProp<glm::ivec2>(Snowglobe::Core::Serialization::SerializationAPI* api, glm::ivec2* value, uint32_t metaFlags)
{
    api->BaseProperty("x", value->x, metaFlags);
    api->BaseProperty("y", value->y, metaFlags);
}

template <>
inline void CustomProp<glm::ivec3>(Snowglobe::Core::Serialization::SerializationAPI* api, glm::ivec3* value, uint32_t metaFlags)
{
    api->BaseProperty("x", value->x, metaFlags);
    api->BaseProperty("y", value->y, metaFlags);
    api->BaseProperty("z", value->z, metaFlags);
}

template <>
inline void CustomProp<glm::ivec4>(Snowglobe::Core::Serialization::SerializationAPI* api, glm::ivec4* value, uint32_t metaFlags)
{
    api->BaseProperty("x", value->x, metaFlags);
    api->BaseProperty("y", value->y, metaFlags);
    api->BaseProperty("z", value->z, metaFlags);
    api->BaseProperty("w", value->w, metaFlags);
}