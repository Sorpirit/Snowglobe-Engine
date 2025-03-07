#pragma once

#include "../RuntimeTest.hpp"

namespace Snowglobe {

class ModelLoading : public RuntimeTest {
  public:
  ModelLoading() : RuntimeTest("ModelLoading") {}
    void Init() override;
    void Run() override;
};

} // Snowglobe