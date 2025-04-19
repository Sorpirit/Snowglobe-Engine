#pragma once

#include <unordered_map>

#include "ECS/Component.hpp"
#include "ECS/ISystem.hpp"
#include "InputReader.hpp"

#include "UISystem.hpp"

namespace Snowglobe::Engine
{

class Engine;

class EngineInspectorSystem : public Core::ECS::ISystem
{
public:
    EngineInspectorSystem(Render::UISystem* uiSystem, Core::InputReader* input, Engine* engine);

    template <class TComponent>
    void RegisterComponentVisualiser(std::function<void(Render::UISystem&, TComponent&)> visualizer)
    {
        const auto& componentType = std::type_index(typeid(TComponent));
        GetOrCreateSimplifiedName(componentType);
        _customComponentVisualisers[componentType] = [=](Render::UISystem* ui, Core::ECS::Component* component) {
            auto tComponent = static_cast<TComponent*>(component);
            visualizer(*ui, *tComponent);
        };
    }

    template <class TSystem>
    void RegisterSystemVisualiser(std::function<void(Render::UISystem&, TSystem&)> visualizer)
    {
        const auto& componentType = std::type_index(typeid(TSystem));
        GetOrCreateSimplifiedName(componentType);
        _customSystemVisualisers[componentType] = [=](Render::UISystem* ui, Core::ECS::ISystem* system) {
            auto tComponent = static_cast<TSystem*>(system);
            visualizer(*ui, *tComponent);
        };
    }

    void Update() override;
    
private:
    std::unordered_map<std::type_index, std::function<void(Render::UISystem*, Core::ECS::Component*)>> _customComponentVisualisers;
    std::unordered_map<std::type_index, std::function<void(Render::UISystem*, Core::ECS::ISystem*)>> _customSystemVisualisers;
    std::unordered_map<std::type_index, std::string> _simplifiedName;

    const std::string& GetOrCreateSimplifiedName(std::type_index type)
    {
        auto value = _simplifiedName.find(type);
        if (value != _simplifiedName.end())
            return value->second;

        std::string const fullName = type.name();
        _simplifiedName[type] = fullName.substr(fullName.find_last_of(':') + 1);
        return _simplifiedName[type];
    }

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