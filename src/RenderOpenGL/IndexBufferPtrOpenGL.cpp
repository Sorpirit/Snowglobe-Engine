#include <IndexBufferPtrOpenGL.hpp>

#include "glad/gl.h"

namespace Snowglobe::RenderOpenGL
{
    IndexBufferPtrOpenGL::IndexBufferPtrOpenGL(const std::vector<uint32_t>& indices, const std::string& debugName) :
        _debugName(debugName)
    {
        glGenBuffers(1, &_bufferPtr);
        UploadData(indices);
    }

    IndexBufferPtrOpenGL::~IndexBufferPtrOpenGL()
    {
        // Todo: delocate buffer other way   
        // glDeleteBuffers(1, &_bufferPtr);
    }

    void IndexBufferPtrOpenGL::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferPtr);
    }

    void IndexBufferPtrOpenGL::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void IndexBufferPtrOpenGL::UploadData(const std::vector<uint32_t>& indices)
    {
        _count = indices.size();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferPtr);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
    }
} // namespace Snowglobe::RenderOpenGL
