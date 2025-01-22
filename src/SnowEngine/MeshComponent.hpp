#pragma once

#include "ECS/Component.hpp"
#include "MeshProxy.hpp"
#include "BasicShapeFactory.hpp"
#include "MaterialBase.hpp"

namespace Snowglobe::SnowEngine
{

class MeshComponent : public SnowCore::ECS::Component
{
public:
    MeshComponent() = default; 
    MeshComponent(Render::MeshProxy* meshProxy) : _meshProxy(meshProxy) {}

    Render::MeshProxy* GetMeshProxy() { return _meshProxy; }
private:
    Render::MeshProxy* _meshProxy = nullptr;

};

class BaseComponentMaterial : public SnowCore::ECS::Component
{
    
public:
    BaseComponentMaterial() = default;
    BaseComponentMaterial(Render::MeshProxy* proxy, Render::MaterialBase* material) : _material(material), _proxy(proxy) {}

    Render::BasicShapeMaterial* GetMaterial() const { return _material.Properties(); }
    Render::MeshProxy* GetProxy() const { return _proxy; }
private:
    Render::MaterialInstance<Render::BasicShapeMaterial> _material;
    Render::MeshProxy* _proxy;

};

} // namespace Snowglobe::SnowEngine
