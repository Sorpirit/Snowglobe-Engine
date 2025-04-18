#include "TextureLitMaterialImpl.hpp"

#include "OpenGLRenderSystem.hpp"
#include "UniformLocationSetter.hpp"

#include <glad/gl.h>

namespace Snowglobe::RenderOpenGL::Materials
{

void TextureLitMaterialImpl::Bind(uint32_t pipelineID)
{
    OpenGLRenderSystem::GetInstance()->GetSkybox().BindSkybox(pipelineID, 4);

    UniformLocationSetter uniformSetter{pipelineID};
    uniformSetter.Set("material.diffuseTexture", _data.diffuseTexture);
    uniformSetter.Set("material.specularTexture", _data.specularTexture);
    uniformSetter.Set("material.aoTexture", _data.aoTexture);
    uniformSetter.Set("material.emissiveTexture", _data.emissiveTexture);
    uniformSetter.Set("material.baseColor", _data.baseColor);
    uniformSetter.Set("material.specularPower", _data.specularPower);
    uniformSetter.Set("material.emissivePower", _data.emissivePower);
}

} // namespace Snowglobe::RenderOpenGL::Materials


