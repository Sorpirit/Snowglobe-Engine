#pragma once
#include <cstdint>
#include <iostream>
#include <string>
#include <typeinfo>

namespace Snowglobe::Core::Serialization
{
class SerializationAPI;
template <typename T>
using CustomPropertyFunc = std::function<bool(SerializationAPI*, T&)>;

class SerializationAPI
{
  public:
    template <typename T, bool Dynamic = false> bool Property(T& value)
    {
        bool result = true;
        if (Dynamic)
        {
            std::string type = typeid(value).name();
            result = Property("Type", type);
            if (!result || !TryPushObject("Data"))
                return false;

            result = PropertyInternal(type, &value);
            PopObject();
        }
        else
            result = PropertyInternal(typeid(T).name(), static_cast<void*>(&value));

        return result;
    }
    template <typename T, bool Dynamic = false> bool Property(const std::string& name, T& value)
    {
        if (!TryPushObject(name))
            return false;

        bool result = Property<T, Dynamic>(value);
        PopObject();
        return result;
    }

    template <typename T, bool Dynamic = false> bool Property(const std::string& name, std::vector<T*>& values)
    {
        uint32_t size = static_cast<uint32_t>(values.size());
        if (!TryPushArray(name, size))
            return false;

        for (uint32_t i = 0; i < size; i++)
        {
            PushArrayItemAt(i);
            Property<T, Dynamic>(*values[i]);
            PopObject();
        }

        PopObject();
        return true;
    }

    template <typename T, bool Dynamic = false>
    bool Property(const std::string& name, std::vector<std::shared_ptr<T>>& value)
    {
        uint32_t size = static_cast<uint32_t>(value.size());
        if (!TryPushArray(name, size))
            return false;

        for (uint32_t i = 0; i < size; i++)
        {
            PushArrayItemAt(i);
            Property<T, Dynamic>(*value[i]);
            PopObject();
        }

        PopObject();
        return true;
    }

    template <typename T> void RegisterCustomProperty(CustomPropertyFunc<T> func)
    {
        _properties[typeid(T).name()] = [=](SerializationAPI* api, void* value) {
            auto val = static_cast<T*>(value);
            return func(api, *val);
        };
    }

    template <> bool Property(const std::string& name, int& value) { return BaseProperty(name, value); }
    template <> bool Property(const std::string& name, uint32_t& value) { return BaseProperty(name, value); }
    template <> bool Property(const std::string& name, float& value) { return BaseProperty(name, value); }
    template <> bool Property(const std::string& name, std::string& value) { return BaseProperty(name, value); }
    template <> bool Property(const std::string& name, bool& value) { return BaseProperty(name, value); }

  protected:
    virtual bool TryPushObject(const std::string& name) = 0;
    virtual bool TryPushArray(const std::string& name, uint32_t& size) = 0;
    virtual void PushArrayItemAt(uint32_t index) = 0;
    virtual void PopObject() = 0;

    virtual bool BaseProperty(const std::string& name, int& value) = 0;
    virtual bool BaseProperty(const std::string& name, float& value) = 0;
    virtual bool BaseProperty(const std::string& name, uint32_t& value) = 0;
    virtual bool BaseProperty(const std::string& name, std::string& value) = 0;
    virtual bool BaseProperty(const std::string& name, bool& value) = 0;

    bool PropertyInternal(const std::string& type, void* ptr)
    {
        auto it = _properties.find(type);
        if (it == _properties.end())
        {
            assert(0 && "Unknown type");
            return false;
        }

        return it->second(this, ptr);
    }

  private:
    typedef std::function<bool(SerializationAPI*, void*)> CustomPropertyFuncWrapper;
    std::unordered_map<std::string, CustomPropertyFuncWrapper> _properties;
};

} // namespace Snowglobe::Core::Serialization