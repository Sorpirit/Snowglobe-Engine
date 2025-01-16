#include "CommonVertexLayoutDescriptor.hpp"

#include "glad/gl.h"

namespace Snowglobe::RenderOpenGL
{
    void PositionVertexLayoutDescriptor::SetupAtributePointers() const
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Render::PositionVertex), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void PositionColorVertexLayoutDescriptor::SetupAtributePointers() const
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Render::PositionColorVertex), (void*)offsetof(Render::PositionColorVertex, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Render::PositionColorVertex), (void*)offsetof(Render::PositionColorVertex, color));
        glEnableVertexAttribArray(1);
    }

    void PositionUVVertexLayoutDescriptor::SetupAtributePointers() const
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Render::PositionUVVertex), (void*)offsetof(Render::PositionUVVertex, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Render::PositionUVVertex), (void*)offsetof(Render::PositionUVVertex, uv));
        glEnableVertexAttribArray(1);
    }

    void PositionNormalUVVertexLayoutDescriptor::SetupAtributePointers() const
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Render::PositionNormalUVVertex), (void*)offsetof(Render::PositionNormalUVVertex, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Render::PositionNormalUVVertex), (void*)offsetof(Render::PositionNormalUVVertex, normal));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Render::PositionNormalUVVertex), (void*)offsetof(Render::PositionNormalUVVertex, uv));
        glEnableVertexAttribArray(2);
    }

    void PositionNormalTangentUVVertexLayoutDescriptor::SetupAtributePointers() const
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Render::PositionNormalTangentUVVertex), (void*)offsetof(Render::PositionNormalTangentUVVertex, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Render::PositionNormalTangentUVVertex), (void*)offsetof(Render::PositionNormalTangentUVVertex, normal));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Render::PositionNormalTangentUVVertex), (void*)offsetof(Render::PositionNormalTangentUVVertex, tangent));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Render::PositionNormalTangentUVVertex), (void*)offsetof(Render::PositionNormalTangentUVVertex, uv));
        glEnableVertexAttribArray(3);
    }

    } // namespace Snowglobe::RenderOpenGL