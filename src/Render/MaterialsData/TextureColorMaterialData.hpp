#pragma once 

#include <glm/glm.hpp>
#include "Texture2DPtr.hpp"

namespace Snowglobe::Render::MaterialsData
{
    struct TextureColorMaterialData
    {
        Texture2DPtr texture = Texture2DPtr::WhiteRBG();
        glm::vec4 color = glm::vec4(1.0f);
    };
} // namespace Snowglobe::Render