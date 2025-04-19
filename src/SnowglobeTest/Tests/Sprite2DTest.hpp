#pragma once

#include "../RuntimeTest.hpp"

namespace Snowglobe {

class Sprite2DTest : public RuntimeTest {
  public:
    Sprite2DTest() : RuntimeTest("Sprite2DTest") {}
    void Init() override;
    void Run() override;
};

} // Snowglobe