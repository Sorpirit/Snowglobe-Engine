#include "PositionVertexLayoutDescriptor.hpp"

#include <glad/gl.h>

namespace Snowglobe::RenderOpenGL
{
    void PositionVertexLayoutDescriptor::SetupAtributePointers() const
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Render::PositionVertex), (void*)0);
        glEnableVertexAttribArray(0);
    }
} // namespace Snowglobe::RenderOpenGL