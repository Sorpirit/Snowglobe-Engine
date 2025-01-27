#include <iostream>
#include <utility>

#include "Engine.hpp"
#include "RenderSystem.hpp"
#include "OpenGLRenderSystem.hpp"
//#include "VulkanRenderingSystem.hpp"
#include "Imgui/ImguiSystem.hpp"
#include "PhysicsEngine2DSystem.hpp"
#include "RenderEngineSyncSystem.hpp"
#include "ComponentEditorSystem.hpp"
#include "ComponentEditor.hpp"
#include "EngineTime.hpp"

namespace Snowglobe::Engine
{
    class LightComponentEditor : public TemplateComponentEditor<RenderOpenGL::LightComponent>
    {

    public:
        LightComponentEditor(Render::UISystem& uiSystem) : TemplateComponentEditor(uiSystem) {}

        void DrawUI(RenderOpenGL::LightComponent* component) override
        {
            _uiSystem.Text("LightComponent");
            _uiSystem.Color("LightColor", &component->GetLightParameters().LightColor);
            _uiSystem.Slider("AmbientIntensity", &component->GetLightParameters().AmbientIntensity, 0.0f, 1.0f);
        }
    };

    Engine::~Engine()
    {
        _systems.clear();
    }

    void Engine::Setup(
        const Core::EngineProfile& profile,
        const Render::WindowParams& windowParams,
        const std::shared_ptr<Core::ECS::EntityManagerBase>& entityManager)
    {
        _entityManager = std::move(entityManager);

        bool success = true;
        switch (profile.preferredRenderEngine)
        {
        case Core::EngineRenderEngine::OpenGL:
            _systems[typeid(Render::RenderSystem)] = std::make_shared<RenderOpenGL::OpenGLRenderSystem>();
            break;
        case Core::EngineRenderEngine::Vulkan:
            //todo disable vulkan until implemented
            //success = TryAddSystem<Render::RenderSystem>(new RenderVulkan::VulkanRenderingSystem());
            std::cout << "Vulkan is not implemented yet" << std::endl;
            break;
        case Core::EngineRenderEngine::DirectX:
            std::cout << "DirectX is not implemented yet" << std::endl;
            break;
        default:
            std::cout << "Unknown render engine" << std::endl;
            break;
        }

        if(!success)
        {
            std::cout << "Failed to add render system" << std::endl;
        }

        Render::RenderSystem* renderSystem = nullptr; 
        if(!QuerySystem<Render::RenderSystem>(renderSystem))
        {
            std::cout << "Failed to get render system" << std::endl;
            return;
        }

        renderSystem->InitializeWindow(windowParams);
        
        _systems[typeid(Render::UISystem)] = std::make_shared<RenderOpenGL::Imgui::ImguiSystem>(renderSystem->GetMainWindow());
        
        Render::UISystem* uiSystem = nullptr;
        if(!QuerySystem<Render::UISystem>(uiSystem))
        {
            std::cout << "Failed to get Imgui system" << std::endl;
            return;
        }
        renderSystem->SetUISystem(uiSystem);

        _systems[typeid(PhysicsEngine2DSystem)] = std::make_shared<PhysicsEngine2DSystem>();
        _systems[typeid(RenderEngineSyncSystem)] = std::make_shared<RenderEngineSyncSystem>();

        auto componentEditorSystem = std::make_shared<ComponentEditorSystem>(*uiSystem);
        _systems[typeid(ComponentEditorSystem)] = componentEditorSystem;

        componentEditorSystem->RegisterVisualiser<DebugComponent, DebugComponentEditor>();
        componentEditorSystem->RegisterVisualiser<Core::TransformComponent, TransformComponentEditor>();
        componentEditorSystem->RegisterVisualiser<Physics2DComponent, Physics2DComponentEditor>();
        componentEditorSystem->RegisterVisualiser<Collider2DComponent, Collider2DComponentEditor>();
        componentEditorSystem->RegisterVisualiser<BaseComponentMaterial, BaseComponentMaterialEditor>();
        componentEditorSystem->RegisterVisualiser<Render::NEdgeShape2DComponent, NEdgeShape2DComponentEditor>();
        
        //OpenGL Only
        componentEditorSystem->RegisterVisualiser<RenderOpenGL::LightComponent, LightComponentEditor>();

        for(const auto& system : _systems | std::views::values)
        {
            system->Init(entityManager);
        }
        
        renderSystem->InitializeRenderScene();
    }

    void Engine::Run()
    {
        Core::EngineTime::GetInstance()->EngineTick();
        for(const auto& system : _systems | std::views::values)
        {
            if(system->IsActive())
                system->UpdateEarly();
        }

        for (auto& updateFunction : _updateCallbacks)
        {
            updateFunction();
        }
        
        for(const auto& system : _systems | std::views::values)
        {
            if(system->IsActive())
                system->Update();
        }
        
        for(const auto& system : _systems | std::views::values)
        {
            if(system->IsActive())
                system->UpdateLate();
        }

        _entityManager->Update();
    }

} // namespace Snowglobe::Engine