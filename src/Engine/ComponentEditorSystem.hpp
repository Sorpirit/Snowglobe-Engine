#pragma once

#include <unordered_map>

#include "ECS/Component.hpp"
#include "ECS/ISystem.hpp"
#include "ECS/EntityManager.hpp"
#include "ComponentEditor.hpp"
#include "InputReader.hpp"

#include "UISystem.hpp"

namespace Snowglobe::Engine
{

class Engine;

class ComponentEditorSystem : public Core::ECS::ISystem
{
public:
    ComponentEditorSystem(Render::UISystem* uiSystem, Core::InputReader* input, Engine* engine) : ISystem(true), _uiSystem(uiSystem), _inputReader(input), _engine(engine) {}

    template <class TVisualiser>
    void RegisterVisualiser()
    {
        static_assert(std::is_base_of_v<ComponentEditor, TVisualiser>, "TVisualiser must derive from ComponentVisualiser");
        auto visualiser = std::make_shared<TVisualiser>(_uiSystem);
        _visualisers[visualiser->GetComponentRuntimeType()] = std::move(visualiser); 
    }

    void Update() override
    {
        if (_inputReader->IsKeyPressed(Core::Key::P))
        {
            PuaseAllSystems(!_allSystemsPuased);
        }

        auto window = _uiSystem->OpenUIPanel("EngineProperties");

        if (_uiSystem->BeginTabBar("EnginePropertiesTabBar"))
        {
            if (_uiSystem->BeginTabBarItem("Systems"))
            {
                _uiSystem->Text("Systems");
                DrawSystemUI();
                _uiSystem->EndTabBarItem();
            }

            if (_uiSystem->BeginTabBarItem("Scene"))
            {
                _uiSystem->Text("Scene");
                DrawSceneUI();
                _uiSystem->EndTabBarItem();
            }

            if (_uiSystem->BeginTabBarItem("Inspector"))
            {
                DrawInspectorUI();
                _uiSystem->EndTabBarItem();
            }

            _uiSystem->EndTabBar();
        }
    }
    
private:
    std::unordered_map<std::type_index, std::shared_ptr<ComponentEditor>> _visualisers;

    size_t _selectedEntity = 0;
    Render::UISystem* _uiSystem;
    Core::InputReader* _inputReader;
    Engine* _engine;
    bool _allSystemsPuased = false;

    void DrawSystemUI();
    void DrawSceneUI();
    void DrawInspectorUI()
    {
        auto& entities = _entityManager->GetAllEntities();

        if (_selectedEntity >= entities.size())
            _selectedEntity = entities.size() - 1;

        std::vector<std::string> componentNames;
        for (auto& entity : entities)
        {
            componentNames.push_back(entity->GetName());

            if (!entity->DrawDebug())
                continue;

            Core::TransformComponent* transform = nullptr;
            if (entity->QueryComponents(transform))
            {
                _uiSystem->AddWorldText(transform->Position, entity->GetName(), { 1.0f, 1.0f, 0.0f }, Render::Alignment::HorizontalCenter | Render::Alignment::VerticalCenter);
            }
        }
        _uiSystem->Combo("Select Entity", &_selectedEntity, componentNames);

        auto selectedEntity = entities[_selectedEntity];

        char buffer[128];
        strncpy_s(buffer, selectedEntity->GetName().c_str(), _TRUNCATE);
        if (_uiSystem->Input("Name", buffer, 128))
        {
            selectedEntity->SetName(buffer);
        }
        _uiSystem->SameLine();
        bool isActive = selectedEntity->IsActive();
        if (_uiSystem->Checkbox("Is Active", &isActive))
        {
            selectedEntity->SetActive(isActive);
        }
        if (_uiSystem->Button("Destroy"))
        {
            selectedEntity->Destroy();
            _selectedEntity--;
        }

        std::vector<Core::ECS::Component*> components;
        selectedEntity->ListAttachedComponents(components);
        for (auto& component : components)
        {
            auto visualiser = _visualisers.find(typeid(*component));
            if (visualiser != _visualisers.end())
            {
                visualiser->second->DrawUI(component);
            }
        }
    }

    void PuaseAllSystems(bool pause);
};
}