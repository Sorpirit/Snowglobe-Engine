#pragma once

#include <typeindex>
#include <typeinfo>

#include "RenderPass.hpp"
#include "FileSystem.hpp"

//#include "OpenGLRenderSystem.hpp"

#include "glad/gl.h"

namespace Snowglobe::RenderOpenGL
{
    class OpenGLRenderSystem;

template <class MaterialType, class VertexType>
class TemplateRenderPass : public RenderPass
{
public:
    TemplateRenderPass(const Core::SnowFileHandle& vertex, Core::SnowFileHandle fragment, bool useLighting = false) :
        RenderPass({typeid(MaterialType), typeid(VertexType)}), _sceneParameters(),
        _useLighting(useLighting)
    {
        auto shaderCompiler = OpenGLRenderSystem::GetInstance()->GetShaderCompiler();
        auto createInfo = GraphicsPipelineCreateInfo();
        createInfo.VertexShader = vertex;
        createInfo.FragmentShader = fragment;
        _shaderProgram = shaderCompiler->GetOrCratePipeline(createInfo);
    }

    virtual void Execute(MeshOpenGL& mesh) override
    {
        _shaderProgram->Bind();
        _sceneParameters.Bind(OpenGLRenderSystem::GetInstance()->GetCamera(), _shaderProgram->GetProgramID());
        if (_useLighting)
            OpenGLRenderSystem::GetInstance()->GetLightParameters().Bind(_shaderProgram->GetProgramID());
        mesh.Bind(_shaderProgram->GetProgramID());
        mesh.Draw();
        mesh.Unbind();
    }

    
protected:
    std::shared_ptr<PipelineProgram> _shaderProgram;
    SceneParameters _sceneParameters;

    bool _useLighting;
};
} // namespace Snowglobe::RenderOpenGL