#pragma once

#include "TextureLitMaterialData.hpp"

namespace Snowglobe::Render::MaterialsData
{
struct TextureLitOutlineMaterialData
{
    TextureLitMaterialData Material;
    glm::vec3 outlineColor;
    float outlineThickness;
};
} // namespace Snowglobe::Render