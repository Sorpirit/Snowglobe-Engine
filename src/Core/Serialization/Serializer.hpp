#pragma once
#include <typeindex>
#include "SerializationAPI.hpp"

namespace Snowglobe::Core::Serialization
{
class Serializer;
template <typename T>
using CustomSerializationFunc = std::function<bool(Serializer*, T&)>;

class Serializer : public SerializationAPI
{
public:
    template <typename T> bool Serialize(T* valRef) { return Serialize(*valRef); }
    template <typename T> bool Serialize(std::shared_ptr<T> valRef) { return Serialize(*valRef.get()); }
    template <typename T> bool Serialize(T& valRef)
    {
        if (auto it = _customSerializers.find(typeid(T).name()); it != _customSerializers.end())
            return it->second(this, static_cast<void*>(&valRef));

        return Property(valRef);
    }

    template <typename T> void RegisterCustomSerializer(CustomSerializationFunc<T> func)
    {
        _customSerializers[typeid(T).name()] = [=](Serializer* api, void* value) {
            func(api, *static_cast<T*>(value));
        };
    }

private:
    typedef std::function<bool(Serializer*, void*)> CustomSerializationFuncWrapper;
    std::unordered_map<std::string, CustomSerializationFuncWrapper> _customSerializers;
};

} // namespace Snowglobe::Core::Serialization