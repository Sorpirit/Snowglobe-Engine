#pragma once

#include <string>

#include "../CachedUniformLocations.hpp"
#include "TemplateMaterialManager.hpp"

namespace Snowglobe::RenderOpenGL::Materials
{
    class BasicShapeMaterialImpl : public TemplateMaterialImpl<Render::BasicShapeMaterial>
    {
    public:
        BasicShapeMaterialImpl(const std::string& debugName) : TemplateMaterialImpl(debugName), _uniformLocations() {}

        void Bind(uint32_t pipelineID) override;

    private:
        CachedUniformLocations<1> _uniformLocations;
    };
}