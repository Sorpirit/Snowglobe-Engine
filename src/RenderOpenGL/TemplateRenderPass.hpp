#pragma once

#include <typeindex>
#include <typeinfo>

#include "RenderPass.hpp"
#include "FileSystem.hpp"

#include "OpenGLRenderSystem.hpp"

#include "glad/gl.h"

namespace Snowglobe::RenderOpenGL
{
template <class MaterialType, class VertexType>
class TemplateRenderPass : public RenderPass
{
public:
    TemplateRenderPass(const Core::SnowFileHandle& vertex, Core::SnowFileHandle fragment, bool useLighting = false) :
        RenderPass({ typeid(MaterialType), typeid(VertexType) }),
        _useLighting(useLighting) 
    {
        auto shaderCompiler = OpenGLRenderSystem::GetInstance()->GetShaderCompiler();
        auto vertexShader = shaderCompiler->GetOrCompileShader(vertex);
        auto fragmentShader = shaderCompiler->GetOrCompileShader(fragment);

        PipelineSetupParams params = {vertexShader, fragmentShader};
        _shaderProgram = shaderCompiler->GetOrCratePipeline(params);
    }
    
    virtual void Execute(MeshOpenGL& mesh) override
    {
        glUseProgram(_shaderProgram);
        _sceneParameters.Bind(OpenGLRenderSystem::GetInstance()->GetCamera(), _shaderProgram);
        if (_useLighting)
            OpenGLRenderSystem::GetInstance()->GetLightParameters().Bind(_shaderProgram);
        mesh.Bind(_shaderProgram);
        mesh.Draw();
        mesh.Unbind();
    }

    
protected:
    uint32_t _shaderProgram;
    SceneParameters _sceneParameters;

    bool _useLighting;
};
} // namespace Snowglobe::RenderOpenGL