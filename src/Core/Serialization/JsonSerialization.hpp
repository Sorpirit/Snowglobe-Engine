#pragma once
#include <stack>

#include <nlohmann/json.hpp>

#include "Deserialize.hpp"
#include "Serializer.hpp"

namespace Snowglobe::Core::Serialization
{

class SAXHandler : public nlohmann::detail::json_sax_dom_parser<nlohmann::json>
{
  public:
    SAXHandler(nlohmann::json& j) : json_sax_dom_parser(j, false) {}

    bool parse_error(std::size_t position, const std::string& last_token, const nlohmann::json::exception& ex)
    {
        std::cerr << "parse error at input byte " << position << "\n"
                  << ex.what() << "\n"
                  << "last read: \"" << last_token << "\"" << std::endl;
        return false;
    }
};

class JsonReader : public Deserializer
{
  public:
    JsonReader(nlohmann::json& j) : original(j), ptr(&original) {}

    bool Contains(const std::string& varName) const { return ptr->contains(varName); }

    bool BaseProperty(const std::string& name, int& value) override { return ReadInternal(name, value); }
    bool BaseProperty(const std::string& name, float& value) override { return ReadInternal(name, value); }
    bool BaseProperty(const std::string& name, std::string& value) override { return ReadInternal(name, value); }
    bool BaseProperty(const std::string& name, bool& value) override { return ReadInternal(name, value); }
    bool BaseProperty(const std::string& name, uint32_t& value) override { return ReadInternal(name, value); }

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
    bool ReadInternal(const std::string varName, auto& var)
    {
        if (ptr->contains(varName))
        {
            var = ptr->at(varName);
            return true;
        }
        else if (ptr->is_primitive())
        {
            var = *ptr;
            return true;
        }

        return false;
    }

    nlohmann::json original;
    nlohmann::json* ptr;
    std::stack<nlohmann::json*> stack;
};

class JsonWriter : public Serializer
{
  public:
    JsonWriter() : original(nlohmann::json::object()), ptr(&original) {}

    bool BaseProperty(const std::string& varName, bool& var) override { return WriteInternal(varName, var); }
    bool BaseProperty(const std::string& varName, int& var) override { return WriteInternal(varName, var); }
    bool BaseProperty(const std::string& varName, uint32_t& var) override { return WriteInternal(varName, var); }
    bool BaseProperty(const std::string& varName, float& var) override { return WriteInternal(varName, var); }
    bool BaseProperty(const std::string& varName, std::string& var) override { return WriteInternal(varName, var); }

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
    bool WriteInternal(const std::string varName, auto& var)
    {
        if (ptr->is_array())
        {
            ptr->push_back(var);
            return true;
        }
        else if (ptr->is_object())
        {
            (*ptr)[varName] = var;
            return true;
        }

        return false;
    }

    nlohmann::json original;
    nlohmann::json* ptr;
    std::stack<nlohmann::json*> stack;
};
} // namespace Snowglobe::Core::Serialization