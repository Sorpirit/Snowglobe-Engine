#include "MeshOpenGL.hpp"

#include "glad/gl.h"

namespace Snowglobe::RenderOpenGL
{

MeshOpenGL::MeshOpenGL(
    const Render::RenderID &id,
    const VertexBufferPtrOpenGL& vertexBuffer,
    const IndexBufferPtrOpenGL& indexBuffer,
    std::string debugName) 
    : MeshProxy(id),
    _debugName(std::move(debugName)),
    _vertexBuffer(&vertexBuffer),
    _vertexType(typeid(*vertexBuffer.GetLayoutDescriptor())),
    _indexBuffer(&indexBuffer),
    _material(nullptr), 
    _materialType(typeid(void))
{
    Init();
}

MeshOpenGL::MeshOpenGL(const Render::RenderID &id, const VertexBufferPtrOpenGL& vertexBuffer, std::string debugName) 
    : MeshProxy(id),
    _debugName(std::move(debugName)),
    _vertexBuffer(&vertexBuffer),
    _vertexType(typeid(*vertexBuffer.GetLayoutDescriptor())),
    _indexBuffer(nullptr),
    _material(nullptr),
    _materialType(typeid(void))
{
    Init();
}

void MeshOpenGL::Bind(uint32_t pipelineId)
{
    _entity.Bind(pipelineId);

    if(_material)
        _material->Bind(pipelineId);
    
    glBindVertexArray(_vao);
    _vertexBuffer->Bind();
}

void MeshOpenGL::Draw() const
{
    
    if(_indexBuffer)
    {
        glDrawElements(GL_TRIANGLES, _indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, _vertexBuffer->GetVertexCount());
    }
}

void MeshOpenGL::Unbind() const
{
    glBindVertexArray(0);
}

void MeshOpenGL::Init()
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    _vertexBuffer->Bind();

    //optional
    if(_indexBuffer)
        _indexBuffer->Bind();
    
    _vertexBuffer->GetLayoutDescriptor()->SetupAtributePointers();
    glBindVertexArray(0);
}

} // namespace Snowglobe::RenderOpenGL
