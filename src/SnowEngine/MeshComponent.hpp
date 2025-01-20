#pragma once

#include "ECS/Component.hpp"

#include <MeshProxy.hpp>

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
} // namespace Snowglobe::SnowEngine
