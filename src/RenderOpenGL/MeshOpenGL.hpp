#pragma once

#include <string>
#include <glm/glm.hpp>
#include "RenderProxy.hpp"
#include "MeshProxy.hpp"

#include "RenderEntity.hpp"

#include "Materials/MaterialBaseOpenGL.hpp"
#include "VertexBufferPtrOpenGL.hpp"
#include "IndexBufferPtrOpenGL.hpp"

namespace Snowglobe::RenderOpenGL
{
    class MeshOpenGL : public Render::MeshProxy
    {
    public:
        MeshOpenGL(const Render::RenderID& id, const VertexBufferPtrOpenGL& vertexBuffer, const IndexBufferPtrOpenGL& indexBuffer,
                   std::string debugName = "MeshProxy[Vertex,Index]");
        MeshOpenGL(const Render::RenderID& id, const VertexBufferPtrOpenGL& vertexBuffer, std::string debugName = "MeshProxy[Vertex,Index]");

        void SetPosition(const glm::vec3& position) override { _entity.SetPosition(position);}
        void SetRotation(const glm::vec3& rotation) override { _entity.SetRotation(rotation); }
        void SetScale(const glm::vec3& scale) override { _entity.SetScale(scale); }
        RenderEntity& GetEntity() { return _entity; }
        
        void SetMaterial(Render::MaterialBase& material) override
        {
            _materialType = typeid(material);
            _material = dynamic_cast<Materials::MaterialBaseOpenGL*>(&material);
        }

        const Materials::MaterialBaseOpenGL* GetMaterial() const { return _material; }
        std::type_index GetMaterialType() const {return _materialType;}
        
        const VertexBufferPtrOpenGL* GetVertexBuffer() const { return _vertexBuffer; }
        const IndexBufferPtrOpenGL* GetIndexBuffer() const { return _indexBuffer; }
        std::type_index GetVertexBufferType() const { return _vertexType; }

        void Bind(uint32_t pipelineId);
        void BindEntity(uint32_t pipelineId) { _entity.Bind(pipelineId); }
        void Draw() const;
        void Unbind() const;

    private:
        std::string _debugName;

        const VertexBufferPtrOpenGL* _vertexBuffer;
        std::type_index _vertexType;
        const IndexBufferPtrOpenGL* _indexBuffer;

        uint32_t _vao;

        RenderEntity _entity;

        Materials::MaterialBaseOpenGL* _material;
        std::type_index _materialType;
        

        void Init();
    };
    
} // namespace Snowglobe::RenderOpenGL
