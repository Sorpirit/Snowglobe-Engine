#pragma once

#include <string>
#include <memory>

#include "MaterialsData/TextureLitMaterialData.hpp"

#include "TemplateMaterialManager.hpp"
#include "CachedUniformLocations.hpp"

#include "Texture.hpp"

namespace Snowglobe::RenderOpenGL::Materials
{

class TextureLitMaterialImpl : public TemplateMaterialImpl<Render::MaterialsData::TextureLitMaterialData>
{
public:
    TextureLitMaterialImpl(const std::string& debugName) : TemplateMaterialImpl(debugName) {}

    void Bind(uint32_t pipelineID) override;

private:
    CachedUniformLocations<4> _uniformLocations;
};

}