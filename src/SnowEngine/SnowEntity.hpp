#pragma once

#include <typeinfo>
#include <typeindex>

#include <glm/glm.hpp>
#include <vector>

namespace Snowglobe::SnowEngine
{
    class SnowEntity;

    class SnowEntityComponent
    {
    public:
        void Init(SnowEntity& entity) 
        {
            _entity = &entity; 
            OnAttach(); 
        }
        
        virtual void OnDetach() {}
        virtual void OnActivate() {}
        virtual void OnDeactivate() {}
        virtual void Update() {}
    
        bool NeedsUpdate() const { return _needsUpdate; }
    protected:
        bool _needsUpdate = false;
        SnowEntity* _entity = nullptr;

        virtual void OnAttach() {}
    };

    class SnowEntity
    {
    public:
        SnowEntity(uint32_t id) : _id(id) {}

        void Update() const
        {
            for(auto component : _components)
            {
                if(component->NeedsUpdate())
                {
                    component->Update();
                }
            }

            //_transformDirty = false;
        }
    
        void SetActive(bool active)
        { 
            _isActive = active;
            
            for(auto component : _components)
            {
                if(_isActive)
                    component->OnActivate();
                else
                    component->OnDeactivate();
            }
        }
        bool IsActive() const { return _isActive; }
        glm::vec3 GetPosition() const { return _position; }
        glm::vec3 GetRotation() const { return _rotation; }
        glm::vec3 GetScale() const { return _scale; }
        bool IsTransformDirty() const { return _transformDirty; }

        void SetPosition(const glm::vec3& position)
        { 
            _position = position;
            _transformDirty = true;
        }
        void SetRotation(const glm::vec3& rotation) 
        {
            _rotation = rotation;
            _transformDirty = true;
        }
        void SetScale(const glm::vec3& scale) 
        { 
            _scale = scale; 
            _transformDirty = true;
        }

        void AddComponent(SnowEntityComponent* component)
        {
            _components.push_back(component);
            component->Init(*this);
        }

        void RemoveComponent(SnowEntityComponent* component)
        {
            auto it = std::find(_components.begin(), _components.end(), component);
            if(it != _components.end())
            {
                _components.erase(it);
                component->OnDetach();
            }
        }

        template<class T>
        bool QueryComponent(T*& component) const
        {
            component = nullptr;
            for (auto& c : _components)
            {
                if (typeid(*c) == typeid(T))
                {
                    component = dynamic_cast<T*>(c);
                    return true;
                }
            }

            return false;
        }

        void Destroy()
        {
            if(_isDestroyed)
                return;

            _isDestroyed = true;
            for(auto component : _components)
            {
                if (component == nullptr)
                    continue;

                component->OnDetach();
            }
                

            _components.clear();
        }

        bool operator==(const SnowEntity& entity) const
        {
            return _id == entity._id;
        }

        struct HashFunction
        {
            size_t operator()(const SnowEntity& entity) const
            {
                return std::hash<uint32_t>()(entity._id);
            }
        };

    private:
        bool _isActive = true;
        bool _isDestroyed = false;
        uint32_t _id = 0;

        bool _transformDirty = true;
        glm::vec3 _position = glm::vec3(0.0f);
        glm::vec3 _rotation = glm::vec3(0.0f);
        glm::vec3 _scale = glm::vec3(1.0f);

        std::vector<SnowEntityComponent*> _components;
    };
    
} // namespace Snowglobe::SnowEngine
