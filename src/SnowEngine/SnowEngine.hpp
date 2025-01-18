#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <typeinfo>
#include "EngineProfile.hpp"
#include "ISnowSystem.hpp"
#include "Window.hpp"
#include "SnowEntity.hpp"

namespace Snowglobe::SnowEngine
{
    
    class SnowEngine
    {
    public:
        static SnowEngine& GetInstance()
        {
            static SnowEngine instance;
            return instance;
        }
        
        ~SnowEngine();

        void Setup(const SnowCore::EngineProfile& profile, const Snowglobe::Render::WindowParams& windowParams);

        void StartFrame() const;
        void Update() const;

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

        SnowEntity& CreateEntity() 
        {
            _entities.emplace_back(_nextEntityID);
            _nextEntityID++;
            return _entities.back();
        }

        SnowEntity* CreateEntityPtr() 
        {
            _entities.emplace_back(_nextEntityID);
            _nextEntityID++;
            return &_entities.back();
        }

        void DestroyEntity(SnowEntity& entity)
        {
            auto it = std::find(_entities.begin(), _entities.end(), entity);
            if(it != _entities.end())
            {
                it->Destroy();
                _entities.erase(it);
            }
        }

    private:
        SnowEngine() = default;

        std::string _applicationName;
        std::unordered_map<std::type_index, SnowCore::ISnowSystem*> _systems;
        std::unordered_set<SnowCore::ISnowFrameSystem*> _frameSystems;

        uint32_t _nextEntityID = 0;
        std::list<SnowEntity> _entities;
    };
}

