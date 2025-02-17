#pragma once

#include <cassert>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace Snowglobe::Core
{

class DependencyManager
{
  public:
    static DependencyManager* Instance()
    {
        static DependencyManager instance;
        return &instance;
    }

    template <typename T> std::shared_ptr<T> RegisterSingle()
    {
        auto result = std::make_shared<T>();
        _single[typeid(T)] = result;
        return result;
    }
    template <typename T> std::shared_ptr<T> RegisterSingle(std::shared_ptr<T> ptr) { _single[typeid(T)] = ptr; return ptr; }
    template <typename From, typename To> void RegisterRelation(std::shared_ptr<To> ptr)
    {
        _single[typeid(From)] = ptr;
    }

    template <typename T> std::shared_ptr<T> Resolve()
    {
        auto entry = _single.find(typeid(T));
        assert(entry != _single.end());
        return std::static_pointer_cast<T>(entry->second);
    }

  private:
    std::unordered_map<std::type_index, std::shared_ptr<void>> _single;
    std::unordered_map<std::type_index, std::shared_ptr<void>> _relation;
};

#define DI Snowglobe::Core::DependencyManager::Instance()

} // namespace Snowglobe::Core