#pragma once

#include "RenderProxy.hpp"
#include "MaterialBase.hpp"

namespace Snowglobe::Render
{

class MeshProxy : public RenderProxy
{
public:
    MeshProxy(const RenderID& id) : RenderProxy(id) {}

    virtual void SetMaterial(MaterialBase& materialId) = 0;
};



}