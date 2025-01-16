#include "TextureShapeMaterialImpl.hpp"

#include <glad/gl.h>

namespace Snowglobe::RenderOpenGL::Materials
{

void TextureShapeMaterialImpl::Bind(uint32_t pipelineID)
{
    auto uniformSetter = _uniformLocations.GetSetter(pipelineID);
    uniformSetter.Set("material.mainTexture", _data.texture);
    uniformSetter.Set("material.color", _data.color);
}

} // namespace Snowglobe::RenderOpenGL::Materials


