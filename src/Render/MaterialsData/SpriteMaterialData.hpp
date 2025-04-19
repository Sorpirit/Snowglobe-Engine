#pragma once
#include "Texture2DPtr.hpp"
#include "glm/glm.hpp"

namespace Snowglobe::Render::MaterialsData
{

const int FLIP_X = 1;
const int FLIP_Y = 2;

struct SpriteMaterialData
{
    Texture2DPtr texture;
    glm::vec4 color = glm::vec4(1.0f);
    glm::vec2 textureOffset = glm::vec2(0);
    glm::vec2 textureScale = glm::vec2(1);
    int flipXY = 0;
};

} // namespace Snowglobe::Render