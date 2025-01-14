#include "VertexBufferPtrOpenGL.hpp"

#include "glad/gl.h"

namespace Snowglobe::RenderOpenGL
{
    VertexBufferPtrOpenGL::VertexBufferPtrOpenGL(const VertexLayoutDescriptor* layoutDescriptor, size_t vertexCount, const void* bufferPtr, const std::string& debugName) 
        : _layoutDescriptor(layoutDescriptor), _vertexCount(vertexCount), _debugName(debugName)
    {
        glGenBuffers(1, &_bufferPtr);
        UploadData(vertexCount, bufferPtr);
    }

    VertexBufferPtrOpenGL::~VertexBufferPtrOpenGL()
    {
        //Todo: delocate buffer other way
        //glDeleteBuffers(1, &_bufferPtr);
    }

    void VertexBufferPtrOpenGL::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, _bufferPtr);
    }

    void VertexBufferPtrOpenGL::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBufferPtrOpenGL::UploadData(size_t vertexCount, const void* bufferPtr)
    {
        _vertexCount = vertexCount;
        glBindBuffer(GL_ARRAY_BUFFER, _bufferPtr);
        uint32_t size = _layoutDescriptor->GetStride() * static_cast<uint32_t>(vertexCount);
        glBufferData(GL_ARRAY_BUFFER, size, bufferPtr, GL_STATIC_DRAW);
    }
}
