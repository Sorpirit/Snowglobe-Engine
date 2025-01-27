#pragma once 

#include <glm/glm.hpp>
#include "Texture2DPtr.hpp"

namespace Snowglobe::Render::MaterialsData
{
    struct TextureLitMaterialData
    {
        Texture2DPtr diffuseTexture;
        Texture2DPtr specularTexture;
        glm::vec4 baseColor = glm::vec4(1.0f);
        int specularPower = 32;
    };
} // namespace Snowglobe::Render