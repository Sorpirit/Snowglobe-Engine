#pragma once

#include <string>

#include "VertexBufferPtr.hpp"
#include "VertexLayoutDescriptor.hpp"

namespace Snowglobe::RenderOpenGL
{
    class VertexBufferPtrOpenGL : public Render::VertexBufferPtr
    {
    public:
        VertexBufferPtrOpenGL(const VertexLayoutDescriptor* layoutDescriptor, size_t vertexCount, const void* bufferPtr, const std::string& debugName);
        ~VertexBufferPtrOpenGL();

        void Bind() const;
        void Unbind() const;

        void UploadData(size_t vertexCount, const void* bufferPtr);
        
        
        uint32_t GetVertexCount() const { return _vertexCount; }
        const VertexLayoutDescriptor* GetLayoutDescriptor() const { return _layoutDescriptor; }
        
    private:
        uint32_t _bufferPtr;
        uint32_t _vertexCount;
        std::string _debugName;

        const VertexLayoutDescriptor* _layoutDescriptor;
    };
} // namespace Snowglobe::RenderOpenGL
