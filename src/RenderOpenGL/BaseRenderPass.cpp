#include "BaseRenderPass.hpp"

#include "glad/gl.h"

#include "OpenGLRenderSystem.hpp"

namespace Snowglobe::RenderOpenGL
{

    BaseRenderPass::BaseRenderPass()
    {
        auto shaderCompiler = OpenGLRenderSystem::GetInstance()->GetShaderCompiler();
        auto vertexShader = shaderCompiler->GetOrCompileShader(SnowCore::SnowFileHandle("color.vert"));
        auto fragmentShader = shaderCompiler->GetOrCompileShader(SnowCore::SnowFileHandle("color.frag"));

        PipelineSetupParams params = {vertexShader, fragmentShader};
        _shaderProgram = shaderCompiler->GetOrCratePipeline(params);
    }

    void BaseRenderPass::Execute(MeshOpenGL &mesh)
    {
        glUseProgram(_shaderProgram);
        _sceneParameters.Bind(OpenGLRenderSystem::GetInstance()->GetCamera(), _shaderProgram);
        mesh.Bind(_shaderProgram);
        mesh.Draw();
        mesh.Unbind();
    }
}

