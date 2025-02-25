#pragma once
#include "SerializationAPI.hpp"

namespace Snowglobe::Core::Serialization
{

class Deserializer;
template <typename CComponent, typename DDeserializer>
concept CustomDeserializer = requires(DDeserializer* deserializer)
{
    { deserializer->template Deserialize<CComponent>(std::declval<Deserializer*>()) } -> std::convertible_to<CComponent*>;
};

class Deserializer : public SerializationAPI
{
public:
    template <typename T = void>
    T* Deserialize()
    {
        auto value = DeserializeInternal(typeid(T).name());
        return static_cast<T*>(value);
    }

    template <typename T = void>
    T* Deserialize(const std::string& name)
    {
        if (!TryPushObject(name))
            return nullptr;
        T* value = Deserialize<T>();
        PopObject();
        return value;
    }

    template <typename T = void>
    void DeserializeArray(const std::string& name)
    {
        uint32_t size;
        if (!TryPushArray(name, size))
            return;

        for (uint32_t i = 0; i < size; i++)
        {
            PushArrayItemAt(i);
            Deserialize<T>();
            PopObject();
        }

        PopObject();
    }

    template <typename CComponent, typename T>
    void RegisterCustomDeserializer(T* ptr)
    {
        _deserializer[typeid(CComponent).name()] = [=](Deserializer* api)
        {
            auto v = ptr->template Deserialize<CComponent>(api);
            return static_cast<void*>(v);
        };
    }

    template <typename CComponent>
    void RegisterCustomDeserializer(std::function<void*(Deserializer*)>& func)
    {
        _deserializer[typeid(CComponent).name()] = func;
    }
private:
    void* DeserializeInternal(const std::string& type)
    {
        auto it = _deserializer.find(type);
        if (it == _deserializer.end())
            return nullptr;

        //Think about it
        //PropertyInternal(type, ptr);
        return it->second(this);
    }
private:
    std::unordered_map<std::string, std::function<void*(Deserializer*)>> _deserializer;
};

template <>
inline void* Deserializer::Deserialize<void>()
{
    std::string type;
    BaseProperty("Type", type);
    if (!TryPushObject("Data"))
        return nullptr;

    auto value = DeserializeInternal(type);
    PopObject();
    return value;
}

} // namespace Snowglobe::Core::Serialization