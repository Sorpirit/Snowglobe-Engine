#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <typeinfo>
#include "EngineProfile.hpp"
#include "ISnowSystem.hpp"
#include "Window.hpp"

namespace Snowglobe::SnowEngine
{
    
    class SnowEngine
    {
    private:
        SnowEngine() = default;

        std::string _applicationName;
        std::unordered_map<std::type_index, SnowCore::ISnowSystem*> _systems;
        std::unordered_set<SnowCore::ISnowFrameSystem*> _frameSystems;
    public:
        
        ~SnowEngine();

        void Setup(const SnowCore::EngineProfile& profile, const Snowglobe::Render::WindowParams& windowParams);
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

            SnowCore::ISnowFrameSystem* frameSystem = dynamic_cast<SnowCore::ISnowFrameSystem*>(ptr);
            if(frameSystem)
            {
                _frameSystems.insert(frameSystem);
            }

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

