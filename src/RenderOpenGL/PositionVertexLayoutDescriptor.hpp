#pragma once

#include "BasicShapeFactory.hpp"
#include "VertexLayoutDescriptor.hpp"

#include "glad/gl.h"

namespace Snowglobe::RenderOpenGL
{

    class PositionVertexLayoutDescriptor : public VertexLayoutDescriptor
    {
    public:
        static const PositionVertexLayoutDescriptor* GetInstance()
        {
            static PositionVertexLayoutDescriptor instance;
            return &instance;
        }

        PositionVertexLayoutDescriptor() : VertexLayoutDescriptor(typeid(Render::PositionVertex)) {}

        void SetupAtributePointers() const override;
        uint32_t GetStride() const override { return sizeof(Render::PositionVertex); }
    };
    

} // namespace Snowglobe::RenderOpenGL