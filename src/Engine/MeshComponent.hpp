#pragma once

#include "ECS/Component.hpp"
#include "MeshProxy.hpp"
#include "BasicShapeFactory.hpp"
#include "MaterialBase.hpp"

namespace Snowglobe::Engine
{

class MeshComponent : public Core::ECS::Component
{
public:
    MeshComponent() = default; 
    MeshComponent(Render::MeshProxy* meshProxy) : _meshProxy(meshProxy) {}

    Render::MeshProxy* GetMeshProxy() const { return _meshProxy; }
private:
    Render::MeshProxy* _meshProxy = nullptr;
};

class ModelComponent : public Core::ECS::Component
{
public:
    ModelComponent() = default;
    ModelComponent(std::vector<Render::MeshProxy*> meshes) : _meshes(meshes) {}

    std::vector<Render::MeshProxy*>& GetMeshProxyLists() { return _meshes; }
private:
    std::vector<Render::MeshProxy*> _meshes;
};

class BaseComponentMaterial : public Core::ECS::Component
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
}