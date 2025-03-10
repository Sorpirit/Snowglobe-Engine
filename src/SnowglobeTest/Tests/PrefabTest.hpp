#pragma once

#include "../RuntimeTest.hpp"

namespace Snowglobe {

class PrefabTest : public RuntimeTest {
  public:
    PrefabTest() : RuntimeTest("PrefabTest") {}
    void Init() override;
    void Run() override;
};

} // Snowglobe