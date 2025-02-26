#include "WorldGridDebugDraw.hpp"

#include "RenderSystem.hpp"
#include "Engine.hpp"

namespace Snowglobe::Engine
{

void WorldGridDebugDraw::Init(std::shared_ptr<Core::ECS::EntityManagerBase> entityManager)
{
    ISystem::Init(entityManager);

    Render::RenderSystem* renderPtr;
    DI->Resolve<Engine>()->GetSystemManager()->QuerySystem(renderPtr);
    _gizmosAPI = renderPtr->GetGizmos();
}

void WorldGridDebugDraw::Update()
{
    _gizmosAPI->AddLine({0, 10, 0}, {0, -10, 0}, {0, 1, 0, 1.0f});
    _gizmosAPI->AddLine({10, 0, 0}, {-10, 0, 0}, {1, 0, 0, 1.0f});
    for (int i = 0; i < 11; ++i)
    {
        if (i == 5)
            continue;

        _gizmosAPI->AddLine({10, i - 5, 0}, {-10, i - 5, 0}, {0.4, 0.3, 0.2, 0.7f});
    }

    for (int i = 0; i < 11; ++i)
    {
        if (i == 5)
            continue;

        _gizmosAPI->AddLine({i - 5, 10, 0}, {i - 5, -10, 0}, {0.4, 0.3, 0.2, 0.7f});
    }

    for (int i = 0; i < 10; ++i)
    {
        _gizmosAPI->AddLine({10, i - 5 + 0.5f, 0}, {-10, i - 5 + 0.5f, 0}, {0.05, 0.05, 0.05, 0.7f});
    }

    for (int i = 0; i < 10; ++i)
    {
        _gizmosAPI->AddLine({i - 5 + 0.5f, 10, 0}, {i - 5 + 0.5f, -10, 0}, {0.05, 0.05, 0.05, 0.7f});
    }
}

} // namespace Snowglobe::Engine
