#pragma once

#include "CommonVertexLayouts.hpp"
#include "VertexLayoutDescriptor.hpp"

namespace Snowglobe::RenderOpenGL
{
    class PositionVertexLayoutDescriptor : public TemplateVertexLayoutDescriptor<Render::PositionVertex>
    {
    public:
        static const PositionVertexLayoutDescriptor* GetInstance()
        {
            static PositionVertexLayoutDescriptor instance;
            return &instance;
        }

        void SetupAtributePointers() const override;
    };

    class PositionColorVertexLayoutDescriptor : public TemplateVertexLayoutDescriptor<Render::PositionColorVertex>
    {
    public:
        static const PositionColorVertexLayoutDescriptor* GetInstance()
        {
            static PositionColorVertexLayoutDescriptor instance;
            return &instance;
        }

        void SetupAtributePointers() const override;
    };

    class PositionUVVertexLayoutDescriptor : public TemplateVertexLayoutDescriptor<Render::PositionUVVertex>
    {
    public:
        static const PositionUVVertexLayoutDescriptor* GetInstance()
        {
            static PositionUVVertexLayoutDescriptor instance;
            return &instance;
        }

        void SetupAtributePointers() const override;
    };

    class PositionNormalUVVertexLayoutDescriptor : public TemplateVertexLayoutDescriptor<Render::PositionNormalUVVertex>
    {
    public:
        static const PositionNormalUVVertexLayoutDescriptor* GetInstance()
        {
            static PositionNormalUVVertexLayoutDescriptor instance;
            return &instance;
        }

        void SetupAtributePointers() const override;
    };

    class PositionNormalTangentUVVertexLayoutDescriptor : public TemplateVertexLayoutDescriptor<Render::PositionNormalTangentUVVertex>
    {
    public:
        static const PositionNormalTangentUVVertexLayoutDescriptor* GetInstance()
        {
            static PositionNormalTangentUVVertexLayoutDescriptor instance;
            return &instance;
        }

        void SetupAtributePointers() const override;
    };
    

} // namespace Snowglobe::RenderOpenGL