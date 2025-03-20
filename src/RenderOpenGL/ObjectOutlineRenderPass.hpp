#pragma once

#include <typeindex>
#include <typeinfo>

#include "RenderPass.hpp"
#include "FileSystem.hpp"

#include "Materials/OutlineTextureLitMaterialImpl.hpp"
#include "MaterialsData/TextureLitMaterialData.hpp"
#include "SceneParameters.hpp"
#include "ShaderCompiler.hpp"
#include "glad/gl.h"

namespace Snowglobe::RenderOpenGL
{
class OpenGLRenderSystem;

class ObjectOutlineRenderPass : public RenderPass
{
public:
    ObjectOutlineRenderPass(RenderPass* litPass) :
        RenderPass({typeid(Materials::OutlineTextureLitMaterialImpl), typeid(PositionNormalUVVertexLayoutDescriptor)}), _litPass(litPass), _sceneParameters()
    {
        auto shaderCompiler = OpenGLRenderSystem::GetInstance()->GetShaderCompiler();
        GraphicsPipelineCreateInfo params;
        params.VertexShader = Core::SnowFileHandle("color.vert");
        params.FragmentShader = Core::SnowFileHandle("color.frag");
        _shaderProgram = shaderCompiler->GetOrCratePipeline(params);
    }

    void Execute(MeshOpenGL& mesh) override
    {
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        _litPass->Execute(mesh);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        _shaderProgram->Bind();
        _sceneParameters.Bind(OpenGLRenderSystem::GetInstance()->GetCamera(), _shaderProgram->GetProgramID());
        auto baseScale= mesh.GetEntity().Scale();
        mesh.SetScale(baseScale + glm::vec3(0.04f));
        mesh.Bind(_shaderProgram->GetProgramID());
        mesh.Draw();
        mesh.Unbind();
        mesh.SetScale(baseScale);
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    }

    
protected:
    std::shared_ptr<PipelineProgram> _shaderProgram;
    SceneParameters _sceneParameters;
    RenderPass* _litPass;
};
} // namespace Snowglobe::RenderOpenGL