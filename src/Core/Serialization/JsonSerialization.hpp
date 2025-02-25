#pragma once
#include <stack>

#include <nlohmann/json.hpp>

#include "SerializationAPI.hpp"
#include "Serializer.hpp"
#include "Deserialize.hpp"

namespace Snowglobe::Core::Serialization
{

class SAXHandler : public nlohmann::detail::json_sax_dom_parser<nlohmann::json>
{
public:
    SAXHandler(nlohmann::json& j)
      : nlohmann::detail::json_sax_dom_parser<nlohmann::json>(j, false)
    {}

    bool parse_error(std::size_t position,
                     const std::string& last_token,
                     const nlohmann::json::exception& ex)
    {
        std::cerr << "parse error at input byte " << position << "\n"
                  << ex.what() << "\n"
                  << "last read: \"" << last_token << "\""
                  << std::endl;
        return false;
    }
};

class JsonReader : public Deserializer
{
  public:
    JsonReader(nlohmann::json& j) : original(j), ptr(&original) {}

    bool Contains(const std::string& varName) { return ptr->contains(varName); }

    void BaseProperty(const std::string& name, int& value, uint32_t metaFlags) override { ReadInternal(name, value); }
    void BaseProperty(const std::string& name, float& value, uint32_t metaFlags) override { ReadInternal(name, value); }
    void BaseProperty(const std::string& name, std::string& value, uint32_t metaFlags) override { ReadInternal(name, value); }
    void BaseProperty(const std::string& name, bool& value, uint32_t metaFlags) override { ReadInternal(name, value); }
    void BaseProperty(const std::string& name, uint32_t& value, uint32_t metaFlags) override { ReadInternal(name, value); }

  protected:
    bool TryPushObject(const std::string& name) override
    {
        if (!ptr->contains(name))
            return false;

        stack.push(ptr);
        ptr = &(*ptr)[name];
        return true;
    }
    bool TryPushArray(const std::string& name, uint32_t& size) override
    {
        if (!ptr->contains(name))
            return false;

        size = ptr->at(name).size();
        TryPushObject(name);
        return true;
    }
    void PushArrayItemAt(uint32_t index) override
    {
        stack.push(ptr);
        ptr = &(*ptr)[index];
    }
    void PopObject() override
    {
        ptr = stack.top();
        stack.pop();
    }

  private:
    void ReadInternal(const std::string varName, auto& var)
    {
        if (ptr->contains(varName))
        {
            var = ptr->at(varName);
        }
        else if (ptr->is_primitive())
        {
            var = *ptr;
        }
    }

    nlohmann::json original;
    nlohmann::json* ptr;
    std::stack<nlohmann::json*> stack;
};

class JsonWriter : public Serializer
{
  public:
    JsonWriter() : original(nlohmann::json::object()), ptr(&original) {}

    void BaseProperty(const std::string& varName, bool& var, uint32_t metaFlags) override { WriteInternal(varName, var); }
    void BaseProperty(const std::string& varName, int& var, uint32_t metaFlags) override { WriteInternal(varName, var); }
    void BaseProperty(const std::string& varName, uint32_t& var, uint32_t metaFlags) override { WriteInternal(varName, var); }
    void BaseProperty(const std::string& varName, float& var, uint32_t metaFlags) override { WriteInternal(varName, var); }
    void BaseProperty(const std::string& varName, std::string& var, uint32_t metaFlags) override { WriteInternal(varName, var); }

    nlohmann::json& GetResult() { return original; }

  protected:
    bool TryPushObject(const std::string& name) override
    {
        stack.push(ptr);
        ptr = &(*ptr)[name];
        *ptr = nlohmann::json::object();
        return true;
    }
    bool TryPushArray(const std::string& name, uint32_t& size) override
    {
        stack.push(ptr);
        ptr = &(*ptr)[name];
        *ptr = nlohmann::json::array();
        return true;
    }
    void PushArrayItemAt(uint32_t index) override
    {
        stack.push(ptr);
        ptr->push_back(nlohmann::json::object());
        ptr = &ptr->back();
    }
    void PopObject() override
    {
        ptr = stack.top();
        stack.pop();
    }

  private:
    void WriteInternal(const std::string varName, auto& var)
    {
        if (ptr->is_array())
        {
            ptr->push_back(var);
        }
        else if (ptr->is_object())
        {
            (*ptr)[varName] = var;
        }
    }

    nlohmann::json original;
    nlohmann::json* ptr;
    std::stack<nlohmann::json*> stack;
};
} // namespace Snowglobe::Core