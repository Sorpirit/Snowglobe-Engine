#pragma once

#include "GizmosAPI.hpp"

#include <array>

#include "glm/glm.hpp"

#include "SceneParameters.hpp"
#include "ShaderCompiler.hpp"
#include "VertexLayoutDescriptor.hpp"

#include <complex.h>

namespace Snowglobe::RenderOpenGL {

struct GizmoVertex
{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec4 color = glm::vec4(1.0f);
};

class GizmoVertexLayoutDescriptor : public TemplateVertexLayoutDescriptor<GizmoVertex>
{
public:
    static const GizmoVertexLayoutDescriptor* GetInstance()
    {
        static GizmoVertexLayoutDescriptor instance;
        return &instance;
    }

    void SetupAtributePointers() const override;
};

class GizmoManager : public Render::GizmosAPI {
public:
    void Init();
    void Draw();
protected:
    void PushVertex(glm::vec3 v, glm::vec4 color, bool fill = true) override
    {
        if (fill)
        {
            _vertices[_vertexCount++] = {v, color};
        }
        else
        {
            _verticesWire[_vertexWireCount++] = {v, color};
        }
    }



private:
    void Flush();

    uint32_t _vbo = 0;
    uint32_t _vao = 0;
    uint32_t _bufferByteSize = 0;
    std::shared_ptr<PipelineProgram> _program = 0;
    SceneParameters _sceneParameters;

    static constexpr size_t BATCH_SIZE = 512;
    uint32_t _vertexCount = 0;
    std::array<GizmoVertex, BATCH_SIZE> _vertices;
    uint32_t _vertexWireCount = 0;
    std::array<GizmoVertex, BATCH_SIZE> _verticesWire;
};

}