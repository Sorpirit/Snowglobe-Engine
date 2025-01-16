#pragma once

#include <typeindex>
#include <typeinfo>

#include "RenderPass.hpp"
#include "SnowFileSystem.hpp"

#include "OpenGLRenderSystem.hpp"

#include "glad/gl.h"

namespace Snowglobe::RenderOpenGL
{
    template <class MaterialType, class VertexType>
    class TemplateRenderPass : public RenderPass
    {
    public:
        TemplateRenderPass(const SnowCore::SnowFileHandle& vertex, SnowCore::SnowFileHandle fragment) : RenderPass({ typeid(MaterialType), typeid(VertexType) }) 
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
            mesh.Bind(_shaderProgram);
            mesh.Draw();
            mesh.Unbind();
        }
    protected:
        uint32_t _shaderProgram;
        SceneParameters _sceneParameters;
    };
} // namespace Snowglobe::RenderOpenGL