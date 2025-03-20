//
// Created by danvu on 2/23/2025.
//

#include "GizmoManager.hpp"

#include "glad/gl.h"
#include "OpenGLRenderSystem.hpp"

namespace Snowglobe::RenderOpenGL {

void GizmoVertexLayoutDescriptor::SetupAtributePointers() const
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), static_cast<void*>(offsetof(GizmoVertex, position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), reinterpret_cast<void*>(offsetof(GizmoVertex, color)));
    glEnableVertexAttribArray(1);
}

void GizmoManager::Init()
{
    auto shaderCompiler = OpenGLRenderSystem::GetInstance()->GetShaderCompiler();
    GraphicsPipelineCreateInfo params;
    params.VertexShader = Core::SnowFileHandle("gizmo.vert");
    params.FragmentShader = Core::SnowFileHandle("gizmo.frag");
    _program = shaderCompiler->GetOrCratePipeline(params);

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    _bufferByteSize = GizmoVertexLayoutDescriptor::GetInstance()->GetStride() * BATCH_SIZE;

    glBufferData(GL_ARRAY_BUFFER, _bufferByteSize * 2, nullptr, GL_STATIC_DRAW); // both filled and wire geometry
    glBufferSubData(GL_ARRAY_BUFFER, 0, _bufferByteSize, _vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, _bufferByteSize, _bufferByteSize, _verticesWire.data());

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    GizmoVertexLayoutDescriptor::GetInstance()->SetupAtributePointers();
    glBindVertexArray(0);

    CreateDiskVertices();
    CreateDiskVerticesWire();
}
void GizmoManager::Draw()
{
    bool needsDraw = _vertexCount > 0 || _vertexWireCount > 0;
    if (!needsDraw)
        return;

    Flush();

    _program->Bind();
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    _sceneParameters.Bind(OpenGLRenderSystem::GetInstance()->GetCamera(), _program->GetProgramID());
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    if (_vertexCount > 0)
        glDrawArrays(GL_TRIANGLES, 0, _vertexCount);
    if (_vertexWireCount > 0)
        glDrawArrays(GL_LINES, BATCH_SIZE, _vertexWireCount);

    glDepthMask(GL_TRUE);
    //End
    _vertexCount = 0;
    _vertexWireCount = 0;
}
void GizmoManager::Flush()
{
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    if (_vertexCount > 0)
        glBufferSubData(GL_ARRAY_BUFFER, 0, GizmoVertexLayoutDescriptor::GetInstance()->GetStride() * _vertexCount, static_cast<const void*>(_vertices.data()));

    if (_vertexWireCount > 0)
        glBufferSubData(GL_ARRAY_BUFFER, _bufferByteSize, GizmoVertexLayoutDescriptor::GetInstance()->GetStride() * _vertexWireCount, static_cast<const void*>(_verticesWire.data()));
}

} // namespace Snowglobe::RenderOpenGL
