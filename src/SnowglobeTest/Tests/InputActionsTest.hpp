#pragma once

#include "../RuntimeTest.hpp"

namespace Snowglobe
{

class TestInputConfig
{
public:
    Engine::InputAction WalkLeft;
    Engine::InputAction WalkRight;
    Engine::InputAction Jump;
};

class InputActionsTest : public RuntimeTest
{
  public:
    InputActionsTest() : RuntimeTest("InputActionsTest") {}
    void Init() override;
    void Run() override;

  private:
    std::shared_ptr<Core::ECS::Entity> Player;
};

} // namespace Snowglobe