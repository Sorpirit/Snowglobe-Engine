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

    void Update() override;
    
private:
    std::unordered_map<std::type_index, std::shared_ptr<ComponentEditor>> _visualisers;

    size_t _selectedEntity = 0;
    Render::UISystem* _uiSystem;
    Core::InputReader* _inputReader;
    Engine* _engine;
    bool _allSystemsPuased = false;

    void DrawSystemUI();
    void DrawSceneUI();
    void DrawInspectorUI();

    void PuaseAllSystems(bool pause);
};
}