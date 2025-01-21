#pragma once

#include <unordered_map>

#include "ECS/Component.hpp"
#include "ECS/ISystem.hpp"
#include "ECS/EntityManager.hpp"
#include "ComponentEditor.hpp"

#include "UISystem.hpp"

namespace Snowglobe::SnowEngine
{
    
class ComponentEditorSystem : public SnowCore::ECS::ISystem
{
public:
    ComponentEditorSystem(SnowCore::ECS::EntityManagerBase& entityManager, Render::UISystem& uiSystem) : ISystem(entityManager), _uiSystem(uiSystem) {}

    template <class TComponent, class TVisualiser>
    void RegisterVisualiser()
    {
        static_assert(std::is_base_of<ComponentEditor, TVisualiser>::value, "TVisualiser must derive from ComponentVisualiser");
        static_assert(std::is_base_of<SnowCore::ECS::Component, TComponent>::value, "TComponent must derive from Component");
        _visualisers[typeid(TComponent)] = std::make_shared<TVisualiser>(_uiSystem);
    }

    void Update() override
    {
        auto& entities = _entityManager.GetAllEntities();

        if (entities.empty())
            return;

        _selectedEntity = std::clamp(_selectedEntity, 0, static_cast<int>(entities.size() - 1));
        
        std::vector<std::string> componentNames;
        for (auto& entity : entities)
        {
            componentNames.push_back(entity->GetName());

            SnowCore::TransformComponent* transform = nullptr;
            DebugComponent* debug = nullptr;
            if (entity->QueryComponents(transform, debug) && debug->DrawEntityName)
            {
                _uiSystem.AddWorldText(transform->Position, entity->GetName(), {1.0f, 1.0f, 0.0f}, Render::Alignment::HorizontalCenter | Render::Alignment::VerticalCenter);
            }
        }
        _uiSystem.Combo("Select Entity", &_selectedEntity, componentNames);

        auto selectedEntity = entities[_selectedEntity];

        char buffer[128];
        strncpy_s(buffer, selectedEntity->GetName().c_str(), _TRUNCATE);
        if (_uiSystem.Input("Name", buffer, 128))
        {
            selectedEntity->SetName(buffer);
        }
        _uiSystem.SameLine();
        bool isActive = selectedEntity->IsActive();
        if (_uiSystem.Checkbox("Is Active", &isActive))
        {
            selectedEntity->SetActive(isActive);
        }
        if (_uiSystem.Button("Destroy"))
        {
            selectedEntity->Destroy();
            _selectedEntity--;
        }

        std::vector<SnowCore::ECS::Component*> components;
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
    
private:
    std::unordered_map<std::type_index, std::shared_ptr<ComponentEditor>> _visualisers;

    int _selectedEntity = 0;
    Render::UISystem& _uiSystem;
};


} // namespace Snowglobe::SnowEngine
