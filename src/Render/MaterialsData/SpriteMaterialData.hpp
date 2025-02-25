#pragma once
#include "Texture2DPtr.hpp"
#include "glm/glm.hpp"

namespace Snowglobe::Render::MaterialsData
{

struct SpriteMaterialData
{
    Texture2DPtr texture;
    glm::vec4 color = glm::vec4(1.0f);
    glm::vec2 textureOffset = glm::ivec2(0);
    glm::vec2 textureScale = glm::ivec2(1);
};

} // namespace Snowglobe::Render