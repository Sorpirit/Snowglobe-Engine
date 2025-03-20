#pragma once

#include "../RuntimeTest.hpp"
#include "CommonVertexLayouts.hpp"

#include <vector>

namespace Snowglobe
{

class Basic3DTest : public RuntimeTest
{
  public:
    static void AddQuad(std::vector<Render::PositionNormalUVVertex>& vector, glm::mat4x4 transform);
    static void AddQuad(std::vector<Render::PositionUVVertex>& vector, glm::mat4x4 transform);

    Basic3DTest() : RuntimeTest("Basic3DTest") {}
    void Init() override;
    void Run() override;
};

} // namespace Snowglobe