#pragma once

#include <cstdint>
#include <MaterialBase.hpp>

namespace Snowglobe::RenderOpenGL::Materials
{
    class MaterialBaseOpenGL : public Render::MaterialBase
    {
    public:
        virtual void Bind(uint32_t pipelineID) = 0;
    };

    template <typename MaterialData>
    class TemplateMaterialImpl : public MaterialBaseOpenGL
    {
    public:
        TemplateMaterialImpl(const std::string& debugName) : _debugName(debugName) {}

        void* GetMaterialDataPtr() const override { return (void*)&_data; }

    protected:
        MaterialData _data;
        std::string _debugName;
    };
    
} // namespace Snowglobe::RenderOpenGL::Materials
