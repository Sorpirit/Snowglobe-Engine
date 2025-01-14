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
        MeshOpenGL(const Render::RenderID& id, const VertexBufferPtrOpenGL& vertexBuffer, const IndexBufferPtrOpenGL& indexBuffer, const std::string& debugName = "MeshProxy[Vertex,Index]");
        MeshOpenGL(const Render::RenderID& id, const VertexBufferPtrOpenGL& vertexBuffer, const std::string& debugName = "MeshProxy[Vertex,Index]");

        void SetPosition(const glm::vec3& position) override { _entity.SetPosition(position);}
        void SetRotation(const glm::vec3& rotation) override { _entity.SetRotation(rotation); }
        void SetScale(const glm::vec3& scale) override { _entity.SetScale(scale); }
        
        void SetMaterial(Render::MaterialBase& material) override { _material = (Materials::MaterialBaseOpenGL*) &material; }

        const Materials::MaterialBaseOpenGL* GetMaterial() const { return _material; }
        const VertexBufferPtrOpenGL* GetVertexBuffer() const { return _vertexBuffer; }
        const IndexBufferPtrOpenGL* GetIndexBuffer() const { return _indexBuffer; }

        void Bind(uint32_t pipelineId);
        void Draw();
        void Unbind() const;

    private:
        std::string _debugName;

        const VertexBufferPtrOpenGL* _vertexBuffer;
        const IndexBufferPtrOpenGL* _indexBuffer;

        uint32_t _vao;

        RenderEntity _entity;

        Materials::MaterialBaseOpenGL* _material;

        void Init();
    };
    
} // namespace Snowglobe::RenderOpenGL
