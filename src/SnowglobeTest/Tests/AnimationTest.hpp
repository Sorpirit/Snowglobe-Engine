#pragma once

#include "../RuntimeTest.hpp"

namespace Snowglobe {

class AnimationTest : public RuntimeTest {
  public:
  AnimationTest() : RuntimeTest("AnimationTest") {}
    void Init() override;
    void Run() override;
private:
    std::shared_ptr<Core::ECS::Entity> Player;
};

} // Snowglobe