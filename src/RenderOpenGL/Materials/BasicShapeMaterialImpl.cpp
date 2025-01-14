#include "BasicShapeMaterialImpl.hpp"

#include <glad/gl.h>

namespace Snowglobe::RenderOpenGL::Materials
{

void BasicShapeMaterialImpl::Bind(uint32_t pipelineID)
{
    auto uniformSetter = _uniformLocations.GetSetter(pipelineID);
    uniformSetter.Set("material.color", _data.color);
}

} // namespace Snowglobe::RenderOpenGL::Materials


