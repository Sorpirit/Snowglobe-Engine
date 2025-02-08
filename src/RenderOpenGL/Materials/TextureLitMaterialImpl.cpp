#include "TextureLitMaterialImpl.hpp"

#include <glad/gl.h>

namespace Snowglobe::RenderOpenGL::Materials
{

void TextureLitMaterialImpl::Bind(uint32_t pipelineID)
{
    auto uniformSetter = _uniformLocations.GetSetter(pipelineID);
    uniformSetter.Set("material.diffuseTexture", _data.diffuseTexture);
    uniformSetter.Set("material.specularTexture", _data.specularTexture);
    uniformSetter.Set("material.emissiveTexture", _data.emissiveTexture);
    uniformSetter.Set("material.baseColor", _data.baseColor);
    uniformSetter.Set("material.specularPower", _data.specularPower);
    uniformSetter.Set("material.emissivePower", _data.emissivePower);
}

} // namespace Snowglobe::RenderOpenGL::Materials


