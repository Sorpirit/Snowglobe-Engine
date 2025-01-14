#pragma once

#include "SnowEntity.hpp"
#include <MeshProxy.hpp>

namespace Snowglobe::SnowEngine
{
    class MeshComponent : public SnowEntityComponent
    {
    public:
        MeshComponent(Render::MeshProxy& meshProxy) : _meshProxy(meshProxy) { _needsUpdate = true; }

        void Update() override
        {
            // if (!_entity->IsTransformDirty())
            //     return;

            _meshProxy.SetPosition(_entity->GetPosition());
            _meshProxy.SetRotation(_entity->GetRotation());
            _meshProxy.SetScale(_entity->GetScale());
        }
    private:
        Render::MeshProxy& _meshProxy;

    };
} // namespace Snowglobe::SnowEngine
