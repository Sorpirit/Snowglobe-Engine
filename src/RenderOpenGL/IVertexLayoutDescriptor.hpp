#pragma once

#include <typeindex>
#include <typeinfo>

namespace Snowglobe::RenderOpenGL
{
    class IVertexLayoutDescriptor
    {
    public:
        IVertexLayoutDescriptor() = default;

        inline virtual void SetupAtributePointers() const = 0; 

        bool ValidateVertexType(std::type_index vertexType) const { return vertexType == _suportedVertexType; }

    protected:
        std::type_index _suportedVertexType = std::type_index(typeid(void));
    };
    
} // namespace Snowglobe::RenderOpenGL
