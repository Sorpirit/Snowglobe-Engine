#include "BasicShapeMaterialImpl.hpp"

#include "UniformLocationSetter.hpp"

#include <glad/gl.h>

namespace Snowglobe::RenderOpenGL::Materials
{

void BasicShapeMaterialImpl::Bind(uint32_t pipelineID)
{
    UniformLocationSetter uniformSetter{pipelineID};
    uniformSetter.Set("material.color", _data.color);
}

} // namespace Snowglobe::RenderOpenGL::Materials


