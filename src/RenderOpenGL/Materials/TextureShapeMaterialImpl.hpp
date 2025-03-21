#pragma once

#include <string>
#include <memory>

#include "MaterialsData/TextureColorMaterialData.hpp"
#include "TemplateMaterialManager.hpp"
#include "Texture.hpp"

namespace Snowglobe::RenderOpenGL::Materials
{

    class TextureShapeMaterialImpl : public TemplateMaterialImpl<Render::MaterialsData::TextureColorMaterialData>
    {
    public:
        TextureShapeMaterialImpl(const std::string& debugName) : TemplateMaterialImpl(debugName) {}

        void Bind(uint32_t pipelineID) override;
    };
}