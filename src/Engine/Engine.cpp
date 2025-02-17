#include <iostream>

#include "Engine.hpp"
#include "OpenGLRenderSystem.hpp"
#include "RenderSystem.hpp"
// #include "VulkanRenderingSystem.hpp"
#include "ComponentEditor.hpp"
#include "ComponentEditorSystem.hpp"
#include "EngineTime.hpp"
#include "Imgui/ImguiSystem.hpp"
#include "LifetimeSystem.hpp"
#include "PhysicsEngine2DSystem.hpp"
#include "RenderEngineSyncSystem.hpp"

namespace Snowglobe::Engine
{
class DirectionalLightComponentEditor : public TemplateComponentEditor<RenderOpenGL::DirectionalLightComponent>
{
  public:
    DirectionalLightComponentEditor(Render::UISystem* uiSystem) : TemplateComponentEditor(uiSystem) {}

    void DrawUITemplate(RenderOpenGL::DirectionalLightComponent* component) override
    {
        _uiSystem->Text("DirectionalLightComponent");
        _uiSystem->Color("Color", &component->Light.LightColor);
        _uiSystem->Slider("AmbientIntensity", &component->Light.AmbientIntensity, 0.0f, 1.0f);
    }
};

class PointLightComponentComponentEditor : public TemplateComponentEditor<RenderOpenGL::PointLightComponent>
{
  public:
    PointLightComponentComponentEditor(Render::UISystem* uiSystem) : TemplateComponentEditor(uiSystem) {}

    void DrawUITemplate(RenderOpenGL::PointLightComponent* component) override
    {
        _uiSystem->Text("DirectionalLightComponent");
        _uiSystem->Color("Color", &component->Light.LightColor);
        _uiSystem->Slider("AmbientIntensity", &component->Light.AmbientIntensity, 0.0f, 1.0f);
        _uiSystem->Slider("AttenuationCoefficients", &component->Light.AttenuationCoefficients, 0.0f, 1.0f);
        _uiSystem->Slider("MaxDistance", &component->Light.MaxDistance, 0.05f, 100.0f);
    }
};

class SpotLightComponentEditor : public TemplateComponentEditor<RenderOpenGL::SpotLightComponent>
{
  public:
    SpotLightComponentEditor(Render::UISystem* uiSystem) : TemplateComponentEditor(uiSystem) {}

    void DrawUITemplate(RenderOpenGL::SpotLightComponent* component) override
    {
        _uiSystem->Text("DirectionalLightComponent");
        _uiSystem->Color("Color", &component->Light.LightColor);
        _uiSystem->Slider("AmbientIntensity", &component->Light.AmbientIntensity, 0.0f, 1.0f);
        _uiSystem->SliderAngle("InnerCutoffAngle", &component->Light.InnerCutoffAngle, 0.0f, 360.0f);
        _uiSystem->SliderAngle("OuterCutoffAngle", &component->Light.OuterCutoffAngle, 0.0f, 360.0f);
    }
};

Engine::~Engine() {}

void Engine::Setup(const Core::EngineProfile& profile, const Render::WindowParams& windowParams,
                   std::shared_ptr<Core::ECS::EntityManagerBase> entityManager)
{
    _entityManager = std::move(entityManager);
    _systemManager = std::make_shared<Core::ECS::SystemManager>(_entityManager);

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
    _systemManager->TryAddSystem<RenderEngineSyncSystem>();
    _systemManager->TryAddSystem<LifetimeSystem>();
    _systemManager->TryAddSystem<LifetimeSystem>();
    _systemManager->TryAddSystem<ComponentEditorSystem>(Core::ECS::DefaultLifetime, uiSystem,
                                                        &renderSystem->GetMainWindow()->GetInput(), this);

    // Currently not used or broken todo fix or remove
    // _systemManager->TryAddSystem<LifeLinkSystem>();
    // _systemManager->TryAddSystem<TweenerSystem>();

    ComponentEditorSystem* componentEditorSystem = nullptr;
    if (!_systemManager->QuerySystem<ComponentEditorSystem>(componentEditorSystem))
    {
        std::cout << "Failed to get Component Editor system" << '\n';
        return;
    }

    componentEditorSystem->RegisterVisualiser<TransformComponentEditor>();
    componentEditorSystem->RegisterVisualiser<Physics2DComponentEditor>();
    componentEditorSystem->RegisterVisualiser<Collider2DComponentEditor>();
    componentEditorSystem->RegisterVisualiser<BaseComponentMaterialEditor>();
    componentEditorSystem->RegisterVisualiser<NEdgeShape2DComponentEditor>();

    // OpenGL Only
    componentEditorSystem->RegisterVisualiser<DirectionalLightComponentEditor>();
    componentEditorSystem->RegisterVisualiser<PointLightComponentComponentEditor>();
    componentEditorSystem->RegisterVisualiser<SpotLightComponentEditor>();

    renderSystem->InitializeRenderScene();
}

void Engine::Run()
{
    Core::EngineTime::GetInstance()->EngineTick();
    _entityManager->Update();
    _systemManager->Update();
}

} // namespace Snowglobe::Engine
