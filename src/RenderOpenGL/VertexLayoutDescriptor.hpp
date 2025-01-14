#pragma once

#include <memory>
#include <typeindex>
#include <typeinfo>

namespace Snowglobe::RenderOpenGL
{
    class VertexLayoutDescriptor
    {
        public:
            VertexLayoutDescriptor(std::type_index) : _vertexType(_vertexType) {}

            virtual inline void SetupAtributePointers() const = 0;
            virtual inline uint32_t GetStride() const = 0;

            inline std::type_index GetVertexType() const { return _vertexType; }

        protected:
            std::type_index _vertexType;
            
    };
    
} // namespace Snowglobe::RenderOpenGL
