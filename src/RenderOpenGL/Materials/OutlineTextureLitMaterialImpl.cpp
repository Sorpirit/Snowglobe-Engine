#include "OutlineTextureLitMaterialImpl.hpp"

#include <glad/gl.h>

namespace Snowglobe::RenderOpenGL::Materials
{

void OutlineTextureLitMaterialImpl::Bind(uint32_t pipelineID)
{
    auto uniformSetter = _uniformLocations.GetSetter(pipelineID);
    uniformSetter.Set("material.diffuseTexture", _data.Material.diffuseTexture);
    uniformSetter.Set("material.specularTexture", _data.Material.specularTexture);
    uniformSetter.Set("material.aoTexture", _data.Material.aoTexture);
    uniformSetter.Set("material.emissiveTexture", _data.Material.emissiveTexture);
    uniformSetter.Set("material.baseColor", _data.Material.baseColor);
    uniformSetter.Set("material.specularPower", _data.Material.specularPower);
    uniformSetter.Set("material.emissivePower", _data.Material.emissivePower);
    uniformSetter.Set("material.color", _data.outlineColor);
}

} // namespace Snowglobe::RenderOpenGL::Materials


