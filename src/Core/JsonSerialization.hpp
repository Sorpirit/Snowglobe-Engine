#pragma once
#include <stack>

#include <nlohmann/json.hpp>

#include "SerializationAPI.hpp"

namespace Snowglobe::Core
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

    bool Contains(const std::string& varName) override { return ptr->contains(varName); }

    void Property(const std::string& varName, bool& var) override { ReadInternal(varName, var); }
    void Property(const std::string& varName, int& var) override
    {
        ReadInternal(varName, var);
        ;
    }
    void Property(const std::string& varName, uint32_t& var) override { ReadInternal(varName, var); }
    void Property(const std::string& varName, float& var) override { ReadInternal(varName, var); }
    void Property(const std::string& varName, std::string& var) override { ReadInternal(varName, var); }

  protected:
    uint32_t GetArraySize(const std::string& varName) override { return ptr->at(varName).size(); }

    void At(const std::string& varName, uint32_t index) override
    {
        stack.push(ptr);
        ptr = &(*ptr)[varName][index];
    }
    void BeginInnerScope(const std::string& varName) override
    {
        stack.push(ptr);
        ptr = &(*ptr)[varName];
    }
    void ExitInnerScope() override
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

    void Property(const std::string& varName, bool& var) override { WriteInternal(varName, var); }
    void Property(const std::string& varName, int& var) override { WriteInternal(varName, var); }
    void Property(const std::string& varName, uint32_t& var) override { WriteInternal(varName, var); }
    void Property(const std::string& varName, float& var) override { WriteInternal(varName, var); }
    void Property(const std::string& varName, std::string& var) override { WriteInternal(varName, var); }

    nlohmann::json& GetResult() { return original; }

  protected:
    void PushSerializationScopeObject(const std::string& varName) override
    {
        stack.push(ptr);
        (*ptr)[varName] = nlohmann::json::object();
        ptr = &(*ptr)[varName];
    }
    void PushSerializationScopeArray(const std::string& varName) override
    {
        stack.push(ptr);
        ptr = &(*ptr)[varName];
        *ptr = nlohmann::json::array();
    }
    void PushSerializationScopeArrayNext() override
    {
        stack.push(ptr);
        ptr->push_back(nlohmann::json::object());
        ptr = &ptr->back();
    }
    void PopSerializationScope() override
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