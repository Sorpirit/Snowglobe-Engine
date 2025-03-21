#pragma once

#include <string>
#include <cstdint>
#include <memory>

#include "MaterialBaseOpenGL.hpp"
#include "MaterialsData/SpriteMaterialData.hpp"

namespace Snowglobe::RenderOpenGL::Materials
{

class SpriteMaterial : public TemplateMaterialImpl<Render::MaterialsData::SpriteMaterialData>
{
public:
    SpriteMaterial(const std::string& debugName) : TemplateMaterialImpl(debugName) {}

    void Bind(uint32_t pipelineID) override;
};

} // namespace Snowglobe::RenderOpenGL::Materials