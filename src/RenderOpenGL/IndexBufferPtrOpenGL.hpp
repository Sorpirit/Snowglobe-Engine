#pragma once

#include <vector>
#include <string>

#include <IndexBufferPtr.hpp>

namespace Snowglobe::RenderOpenGL
{
    class IndexBufferPtrOpenGL : public Render::IndexBufferPtr
    {
    public:
        IndexBufferPtrOpenGL(const std::vector<uint32_t>& indices, const std::string& debugName);
        ~IndexBufferPtrOpenGL();

        void Bind() const;
        void Unbind() const;

        void UploadData(const std::vector<uint32_t>& indices);
        
        uint32_t GetCount() const { return _count; }
        
    private:
        uint32_t _bufferPtr;
        uint32_t _count;

        std::string _debugName;
    };
} // namespace Snowglobe::RenderOpenGL
