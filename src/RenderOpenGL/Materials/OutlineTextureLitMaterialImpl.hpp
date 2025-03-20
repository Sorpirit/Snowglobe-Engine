#pragma once

#include <memory>
#include <string>

#include "MaterialsData/TextureLitOutlineMaterialData.hpp"

#include "CachedUniformLocations.hpp"
#include "TemplateMaterialManager.hpp"

#include "Texture.hpp"

namespace Snowglobe::RenderOpenGL::Materials
{

class OutlineTextureLitMaterialImpl : public TemplateMaterialImpl<Render::MaterialsData::TextureLitOutlineMaterialData>
{
  public:
    OutlineTextureLitMaterialImpl(const std::string& debugName) : TemplateMaterialImpl(debugName) {}

    void Bind(uint32_t pipelineID) override;

  private:
    CachedUniformLocations<8> _uniformLocations;
};

} // namespace Snowglobe::RenderOpenGL::Materials