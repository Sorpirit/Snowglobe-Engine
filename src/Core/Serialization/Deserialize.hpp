#pragma once
#include "SerializationAPI.hpp"

namespace Snowglobe::Core::Serialization
{

class Deserializer;
template <typename T>
using CustomDeserializationFunc = std::function<T&(Deserializer*)>;

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

    template <typename T> void RegisterCustomDeserializer(CustomDeserializationFunc<T> func)
    {
        _customDeserializer[typeid(T).name()] = [=](Deserializer* api) {
            auto& value = func(api);
            return static_cast<void*>(&value);
        };
    }
private:
    void* DeserializeInternal(const std::string& type)
    {
        auto it = _customDeserializer.find(type);
        if (it == _customDeserializer.end())
            return nullptr;

        return it->second(this);
    }

    typedef std::function<void*(Deserializer*)> CustomDeserializationFuncWrapper;
    std::unordered_map<std::string, CustomDeserializationFuncWrapper> _customDeserializer;
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