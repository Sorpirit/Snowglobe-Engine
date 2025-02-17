#pragma once

#include <memory>
#include <typeindex>
#include <typeinfo>

namespace Snowglobe::RenderOpenGL
{
    class VertexLayoutDescriptor
    {
        public:
            VertexLayoutDescriptor(std::type_index vertexType) : _vertexType(vertexType) {}

            virtual inline void SetupAtributePointers() const = 0;
            virtual inline uint32_t GetStride() const = 0;

            std::type_index GetVertexType() const { return _vertexType; }

        protected:
            std::type_index _vertexType;
            
    };

    template <typename T>
    class TemplateVertexLayoutDescriptor : public VertexLayoutDescriptor
    {
        public:
            TemplateVertexLayoutDescriptor() : VertexLayoutDescriptor(typeid(T)) {}

            uint32_t GetStride() const override { return sizeof(T); }
    };
    
} // namespace Snowglobe::RenderOpenGL
