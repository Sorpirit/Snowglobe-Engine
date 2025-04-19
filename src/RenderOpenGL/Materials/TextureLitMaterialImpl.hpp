#pragma once

#include <string>

#include "MaterialsData/TextureLitMaterialData.hpp"
#include "TemplateMaterialManager.hpp"
#include "Texture.hpp"

namespace Snowglobe::RenderOpenGL::Materials
{

class TextureLitMaterialImpl : public TemplateMaterialImpl<Render::MaterialsData::TextureLitMaterialData>
{
  public:
    TextureLitMaterialImpl(const std::string& debugName) : TemplateMaterialImpl(debugName) {}

    void Bind(uint32_t pipelineID) override;
};

} // namespace Snowglobe::RenderOpenGL::Materials