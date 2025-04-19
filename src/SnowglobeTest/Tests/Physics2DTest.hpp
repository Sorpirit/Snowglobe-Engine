#pragma once

#include "../RuntimeTest.hpp"

namespace Snowglobe
{

class Physics2DTest : public RuntimeTest
{
  public:
    Physics2DTest() : RuntimeTest("Physics2DTest") {}
    void Init() override;
    void Run() override;

  private:
    std::shared_ptr<Core::ECS::Entity> _playerBlock;
};

} // namespace Snowglobe