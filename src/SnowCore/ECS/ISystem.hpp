#pragma once

#include "EntityManager.hpp"

namespace Snowglobe::SnowCore::ECS
{

class ISystem
{
public:
    ISystem(EntityManagerBase& entityManager) : _entityManager(entityManager) {}

    virtual void UpdateEarly() { }
    virtual void Update() { }
    virtual void UpdateLate() { }
    

    bool IsActive() const { return _isActive; }
    void SetActive(bool isActive) { _isActive = isActive; }
    
protected:
    EntityManagerBase& _entityManager;

private:
    bool _isActive = true;
};
    
} // namespace Snowglobe::SnowCore::ECS
