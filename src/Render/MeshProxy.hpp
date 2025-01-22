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

    void SetVisible(bool isVisible) { _isVisible = isVisible; }
    bool IsVisible() const { return _isVisible; }
    
protected:
    bool _isVisible = true;
};



}