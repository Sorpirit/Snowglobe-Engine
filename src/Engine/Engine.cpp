#include <iostream>

#include "Engine.hpp"
#include "OpenGLRenderSystem.hpp"
#include "RenderSystem.hpp"
// #include "VulkanRenderingSystem.hpp"
#include "Editor/EngineInspectorSystem.hpp"
#include "Editor/SpectatorCameraSystem.hpp"
#include "Editor/WorldGridDebugDraw.hpp"
#include "EngineTime.hpp"
#include "Imgui/ImguiSystem.hpp"
#include "InputActionSystem.hpp"
#include "LifetimeSystem.hpp"
#include "PhysicsEngine2DSystem.hpp"
#include "RenderEngineSyncSystem.hpp"
#include "SpriteAnimationSystem.hpp"

namespace Snowglobe::Engine
{

Engine::~Engine() {}

void Engine::Setup(const Core::EngineProfile& profile, const Render::WindowParams& windowParams,
                   std::shared_ptr<Core::ECS::EntityManagerBase> entityManager)
{
    _entityManager = std::move(entityManager);
    _systemManager = std::make_shared<Core::ECS::SystemManager>(_entityManager);
    _lifetimeSystem.Init(_entityManager, _systemManager);

    switch (profile.preferredRenderEngine)
    {
    case Core::EngineRenderEngine::OpenGL:
        _systemManager->TryAddSystem<Render::RenderSystem>(std::make_shared<RenderOpenGL::OpenGLRenderSystem>());
        break;
    case Core::EngineRenderEngine::Vulkan:
        // todo disable vulkan until implemented
        // success = TryAddSystem<Render::RenderSystem>(new RenderVulkan::VulkanRenderingSystem());
        std::cout << "Vulkan is not implemented yet" << '\n';
        break;
    case Core::EngineRenderEngine::DirectX:
        std::cout << "DirectX is not implemented yet" << '\n';
        break;
    }

    Render::RenderSystem* renderSystem = nullptr;
    if (!_systemManager->QuerySystem<Render::RenderSystem>(renderSystem))
    {
        std::cout << "Failed to get render system" << '\n';
        return;
    }

    renderSystem->InitializeWindow(windowParams);

    _systemManager->TryAddSystem<Render::UISystem>(
        std::make_shared<RenderOpenGL::Imgui::ImguiSystem>(renderSystem->GetMainWindow()));

    Render::UISystem* uiSystem = nullptr;
    if (!_systemManager->QuerySystem<Render::UISystem>(uiSystem))
    {
        std::cout << "Failed to get UI system" << '\n';
        return;
    }
    renderSystem->SetUISystem(uiSystem);

    _systemManager->TryAddSystem<PhysicsEngine2DSystem>();
    _systemManager->TryAddSystem<PhysicsEngine2DDebugGizmos>();
    _systemManager->TryAddSystem<WorldGridDebugDraw>();
    _systemManager->TryAddSystem<RenderEngineSyncSystem>();
    _systemManager->TryAddSystem<LifetimeSystem>();
    _systemManager->TryAddSystem<Render::SpriteAnimationSystem>();
    _systemManager->TryAddSystem<EngineInspectorSystem>(Core::ECS::DefaultLifetime, uiSystem,
                                                        &renderSystem->GetMainWindow()->GetInput(), this);
    _systemManager->TryAddSystem<SpectatorCameraSystem>(Core::ECS::DefaultLifetime, &renderSystem->GetCamera(),
                                                        &renderSystem->GetMainWindow()->GetInput());

    _inputActionSystem.Init(renderSystem->GetMainWindow()->GetInput());

    EngineInspectorSystem* componentEditorSystem = nullptr;
    if (!_systemManager->QuerySystem<EngineInspectorSystem>(componentEditorSystem))
    {
        std::cout << "Failed to get Component Editor system" << '\n';
        return;
    }

    renderSystem->InitializeRenderScene();
}

void Engine::Run()
{
    Core::EngineTime::GetInstance()->EngineTick();
    _entityManager->Update();
    _systemManager->Update();
}

} // namespace Snowglobe::Engine
