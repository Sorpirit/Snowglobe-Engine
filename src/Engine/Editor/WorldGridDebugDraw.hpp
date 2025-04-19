#pragma once
#include "ECS/ISystem.hpp"
#include "GizmosAPI.hpp"

namespace Snowglobe::Engine
{

class WorldGridDebugDraw : public Core::ECS::ISystem
{
  public:
    void Init(std::shared_ptr<Core::ECS::EntityManagerBase> entityManager) override;
    void Update() override;

  private:
    Render::GizmosAPI* _gizmosAPI;
};

} // namespace Snowglobe::Engine