#pragma once

#include <utility>

#include "EntityManager.hpp"

namespace Snowglobe::Core::ECS
{

/// @brief Base class for all systems in the ECS. Provides a basic interface for updating the system.
class ISystem
{
  public:
    ISystem() = default;
    ISystem(bool isPermanent) : _isPermanent(isPermanent) {}
    virtual ~ISystem() = default;

    virtual void Init(std::shared_ptr<EntityManagerBase> entityManager) { _entityManager = std::move(entityManager); }

    virtual void UpdateEarly() {}
    virtual void Update() {}
    virtual void UpdateLate() {}

    virtual void DrawDebugUI() {}

    bool IsPermanent() const { return _isPermanent; }
    bool IsActive() const { return _isActive; }
    void SetActive(bool isActive)
    {
        if (_isPermanent)
            return;
        _isActive = isActive;
    }

  protected:
    std::shared_ptr<EntityManagerBase> _entityManager = nullptr;

  private:
    bool _isPermanent = false;
    bool _isActive = true;
};

} // namespace Snowglobe::Core::ECS