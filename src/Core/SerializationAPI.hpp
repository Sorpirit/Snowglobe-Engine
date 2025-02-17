#pragma once
#include "glm/glm.hpp"
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

namespace Snowglobe::Core
{

class SerializationAPI
{
  protected:
    void Property(auto& varRef);

    void Property(const std::string& varName, auto& varRef)
    {
        const auto f = [&]() { Property(varRef); };
        ComplexPropertyInternal(varName, f);
    }
    template <typename T> void Property(const std::string& varName, std::vector<T>& varRef)
    {
        const auto complexPropertyAt = [&](uint32_t index) { Property(varRef[index]); };
        const auto resize = [&](uint32_t size) { varRef.resize(size); };
        ComplexPropertyArrayInternal(varName, complexPropertyAt, resize, varRef.size(), std::is_fundamental_v<T>);
    }
    virtual void Property(const std::string& varName, bool& var) = 0;
    virtual void Property(const std::string& varName, int& var) = 0;
    virtual void Property(const std::string& varName, uint32_t& var) = 0;
    virtual void Property(const std::string& varName, float& var) = 0;
    virtual void Property(const std::string& varName, std::string& var) = 0;

    virtual void ComplexPropertyInternal(const std::string& varName, const std::function<void()>& complexProperty) = 0;
    virtual void ComplexPropertyArrayInternal(const std::string& varName,
                                              const std::function<void(uint32_t)>& complexPropertyAt,
                                              const std::function<void(uint32_t)>& resize, uint32_t size,
                                              bool isPrimitive) = 0;
};

template <> inline void SerializationAPI::Property<bool>(bool& varRef) { Property("", varRef); }

template <> inline void SerializationAPI::Property<int>(int& varRef) { Property("", varRef); }

template <> inline void SerializationAPI::Property<uint32_t>(uint32_t& varRef) { Property("", varRef); }

template <> inline void SerializationAPI::Property<float>(float& varRef) { Property("", varRef); }

template <> inline void SerializationAPI::Property<std::string>(std::string& varRef) { Property("", varRef); }

template <> inline void SerializationAPI::Property<glm::vec2>(glm::vec2& varRef)
{
    Property("X", varRef.x);
    Property("Y", varRef.y);
}

template <> inline void SerializationAPI::Property<glm::vec3>(glm::vec3& varRef)
{
    Property("X", varRef.x);
    Property("Y", varRef.y);
    Property("Z", varRef.y);
}

template <> inline void SerializationAPI::Property<glm::vec4>(glm::vec4& varRef)
{
    Property("X", varRef.x);
    Property("Y", varRef.y);
    Property("Z", varRef.z);
    Property("W", varRef.w);
}

class Deserializer : public SerializationAPI
{
  public:
    void Deserialize(auto& varRef) { Property(varRef); }

  protected:
    void ComplexPropertyInternal(const std::string& varName, const std::function<void()>& complexProperty) override
    {
        if (!Contains(varName))
            return;

        BeginInnerScope(varName);
        complexProperty();
        ExitInnerScope();
    }

    void ComplexPropertyArrayInternal(const std::string& varName,
                                      const std::function<void(uint32_t)>& complexPropertyAt,
                                      const std::function<void(uint32_t)>& resize, uint32_t size,
                                      bool isPrimitive) override
    {
        if (!Contains(varName))
            return;

        const uint32_t arraySize = GetArraySize(varName);
        resize(arraySize);
        for (uint32_t i = 0; i < arraySize; ++i)
        {
            At(varName, i);
            complexPropertyAt(i);
            ExitInnerScope();
        }
    }
    virtual bool Contains(const std::string& varName) = 0;
    virtual uint32_t GetArraySize(const std::string& varName) = 0;
    virtual void At(const std::string& varName, uint32_t index) = 0;

    virtual void BeginInnerScope(const std::string& varName) = 0;
    virtual void ExitInnerScope() = 0;
};

class Serializer : public SerializationAPI
{
  public:
    void Serialize(auto& varRef) { Property(varRef); }

  protected:
    void ComplexPropertyInternal(const std::string& varName, const std::function<void()>& complexProperty) override
    {
        PushSerializationScopeObject(varName);
        complexProperty();
        PopSerializationScope();
    }
    void ComplexPropertyArrayInternal(const std::string& varName,
                                      const std::function<void(uint32_t)>& complexPropertyAt,
                                      const std::function<void(uint32_t)>& resize, uint32_t size,
                                      bool isPrimitive) override
    {
        PushSerializationScopeArray(varName);
        for (uint32_t i = 0; i < size; ++i)
        {
            if (!isPrimitive)
                PushSerializationScopeArrayNext();

            complexPropertyAt(i);

            if (!isPrimitive)
                PopSerializationScope();
        }
        PopSerializationScope();
    }

    virtual void PushSerializationScopeObject(const std::string& varName) = 0;
    virtual void PushSerializationScopeArray(const std::string& varName) = 0;
    virtual void PushSerializationScopeArrayNext() = 0;
    virtual void PopSerializationScope() = 0;
};
} // namespace Snowglobe::Core
