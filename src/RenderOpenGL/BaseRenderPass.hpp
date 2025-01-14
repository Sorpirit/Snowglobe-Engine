#pragma once

#include "RenderPass.hpp"
#include "SceneParameters.hpp"
#include "Materials/BasicShapeMaterialImpl.hpp"
#include "PositionVertexLayoutDescriptor.hpp"

namespace Snowglobe::RenderOpenGL
{
    
    class BaseRenderPass : public TemplateRenderPass<Materials::BasicShapeMaterialImpl, PositionVertexLayoutDescriptor>
    {
    public:
        BaseRenderPass();

        void Execute(MeshOpenGL& mesh) override;
    
    private:
        uint32_t _shaderProgram;

        SceneParameters _sceneParameters;
    };

} // namespace Snowglobe::RenderOpenGL
