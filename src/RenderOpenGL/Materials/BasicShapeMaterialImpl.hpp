#pragma once

#include <string>

#include "TemplateMaterialManager.hpp"

namespace Snowglobe::RenderOpenGL::Materials
{
    class BasicShapeMaterialImpl : public TemplateMaterialImpl<Render::BasicShapeMaterial>
    {
    public:
        BasicShapeMaterialImpl(const std::string& debugName) : TemplateMaterialImpl(debugName) {}

        void Bind(uint32_t pipelineID) override;
    };
}