#pragma once

#include <utility>

#include "EntityManager.hpp"
#include "UpdateOrder.hpp"

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

    virtual void Update() = 0;

    bool IsPermanent() const { return _isPermanent; }
    bool IsActive() const { return _isActive; }
    void SetActive(bool isActive)
    {
        if (_isPermanent)
            return;
        _isActive = isActive;
    }

    void UnregisterSystem() { _needToUnregisterSystem = true; }
    bool NeedsUnregisterSystem() const { return _needToUnregisterSystem; }

    uint32_t GetUpdateOrder() const { return _updateOrder; }

  protected:
    std::shared_ptr<EntityManagerBase> _entityManager = nullptr;
    uint32_t _updateOrder = UpdateOrder::PrePhysics;

  private:
    bool _isPermanent = false;
    bool _isActive = true;
    bool _needToUnregisterSystem = false;
};

using FAnonymousUpdate = std::function<void(const std::shared_ptr<EntityManagerBase>&)>;
class AnonymousSystem : public ISystem
{
public:
    AnonymousSystem(std::string debugName, FAnonymousUpdate func) : _debugName(debugName), _func(func) {}

    void Update() override { _func(_entityManager); }

    const std::string& GetDebugName() const { return _debugName; }
private:
    std::string _debugName;
    FAnonymousUpdate _func;
};

} // namespace Snowglobe::Core::ECS