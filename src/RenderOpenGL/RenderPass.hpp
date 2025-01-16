#pragma once

#include <typeindex>
#include <typeinfo>

#include "MeshOpenGL.hpp"

namespace Snowglobe::RenderOpenGL
{
    struct RenderPassSignature
    {
        std::type_index materialType;
        std::type_index vertexType;

        RenderPassSignature(const MeshOpenGL& mesh) : materialType(typeid(*(mesh.GetMaterial()))), vertexType(typeid(*mesh.GetVertexBuffer()->GetLayoutDescriptor())) {}
        RenderPassSignature(std::type_index materialType, std::type_index vertexType) : materialType(materialType), vertexType(vertexType) {}

        bool operator==(const RenderPassSignature& otherId) const
        {
            return materialType == otherId.materialType && vertexType == otherId.vertexType;
        }
    };
    

    class RenderPass
    {
    public:
        RenderPass(const RenderPassSignature& signature) : _signature(signature) {}

        virtual void Execute(MeshOpenGL& mesh) = 0;
        const RenderPassSignature& GetSignature() const { return _signature; }

    protected:
        RenderPassSignature _signature;
    };

} // namespace Snowglobe::RenderOpenGL

template<> struct std::hash<Snowglobe::RenderOpenGL::RenderPassSignature> {
    std::size_t operator()(Snowglobe::RenderOpenGL::RenderPassSignature const& signature) const noexcept {
        return std::hash<std::type_index>()(signature.materialType) ^ std::hash<std::type_index>()(signature.vertexType);
    }
};