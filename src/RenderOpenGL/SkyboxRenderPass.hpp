#pragma once

#include "CachedUniformLocations.hpp"
#include "ShaderCompiler.hpp"
#include "glad/gl.h"

#include <complex.h>
#include <glm/glm.hpp>

namespace Snowglobe::RenderOpenGL
{
class SkyboxRenderPass
{
public:
    void Init();
    void Draw();

    void BindSkybox(uint32_t pipelineId, uint32_t textureIndex);

  private:
    CachedUniformLocations<2> _uniformLocations;

    uint32_t _cubeMapTexture = 0;
    uint32_t _quadVAO = 0;
    std::shared_ptr<PipelineProgram> _quadProgram = 0;
};
}