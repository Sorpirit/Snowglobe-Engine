#pragma once

#include <string>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include "EngineProfile.hpp"
#include "ISnowSystem.hpp"

namespace Snowglobe::SnowEngine
{
    
    class SnowEngine
    {
    private:
        SnowEngine() = default;

        std::string _applicationName;
        std::unordered_map<std::type_index, SnowCore::ISnowSystem*> _systems;
    public:
        
        ~SnowEngine();

        void Setup(const SnowCore::EngineProfile& profile);
        void Update();

        template <typename T>
        bool TryAddSystem(T* ptr)
        {
            static_assert(std::is_base_of<SnowCore::ISnowSystem, T>::value, "T must derive from ISnowSystem");
            auto entry = _systems.find(typeid(T));
            if(entry != _systems.end())
            {
                return false;
            }

            _systems[typeid(T)] = ptr;

            return true;
        }
        
        template <typename T>
        bool QuerySystem(T*& ptr)
        {
            static_assert(std::is_base_of<SnowCore::ISnowSystem, T>::value, "T must derive from ISnowSystem");
            
            auto entry = _systems.find(typeid(T));
            
            if(entry != _systems.end())
            {
                ptr = dynamic_cast<T*>(entry->second);
                return true;
            }

            return false;
        }

        static SnowEngine* GetInstance()
        {
            static SnowEngine _instance;
            return &_instance;
        }
    };
}

