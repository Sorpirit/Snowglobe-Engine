//
// Created by danvu on 2/19/2025.
//

#include "SpriteMaterial.hpp"

#include "UniformLocationSetter.hpp"

namespace Snowglobe {

void RenderOpenGL::Materials::SpriteMaterial::Bind(uint32_t pipelineID)
{
    UniformLocationSetter uniformSetter{pipelineID};
    uniformSetter.Set("material.mainTexture", _data.texture);
    uniformSetter.Set("material.color", _data.color);
    uniformSetter.Set("material.textureOffset", _data.textureOffset);
    uniformSetter.Set("material.textureScale", _data.textureScale);
    uniformSetter.Set("material.flipXY", _data.flipXY);
}

} // Snowglobe