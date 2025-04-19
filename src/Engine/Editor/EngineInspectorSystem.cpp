#include "EngineInspectorSystem.hpp"

#include "CommonVisualizers.hpp"
#include "Engine.hpp"

#include <string>

namespace Snowglobe::Engine
{

EngineInspectorSystem::EngineInspectorSystem(Render::UISystem* uiSystem, Core::InputReader* input, Engine* engine)
    : ISystem(true), _uiSystem(uiSystem), _inputReader(input), _engine(engine)
{
    CommonVisualizers::RegisterVisualizers(this);
}

void EngineInspectorSystem::Update()
{
    if (_inputReader->IsKeyPressed(Core::Key::P))
    {
        PuaseAllSystems(!_allSystemsPuased);
    }

    auto window = _uiSystem->OpenUIPanel("EngineProperties");
    if (_uiSystem->BeginTreeNode("Systems"))
    {
        DrawSystemUI();
        _uiSystem->EndTreeNode();
    }

    if (_uiSystem->BeginTreeNode("Scene"))
    {
        DrawSceneUI();
        _uiSystem->EndTreeNode();
    }

    if (_uiSystem->BeginTreeNode("Inspector"))
    {
        DrawInspectorUI();
        _uiSystem->EndTreeNode();
    }
}

void EngineInspectorSystem::DrawSystemUI()
{
    bool isActive = true;

    std::string buttonText = _allSystemsPuased ? "Unpause" : "Pause";
    if (_uiSystem->Button(buttonText))
    {
        PuaseAllSystems(!_allSystemsPuased);
    }

    for (auto& systemPair : _engine->GetSystemManager()->GetTypedSystems())
    {
        const auto& name = GetOrCreateSimplifiedName(systemPair.first);
        bool isOpen = _uiSystem->BeginTreeNode(name);
        if (!systemPair.second->IsPermanent())
        {
            bool isActive = systemPair.second->IsActive();
            _uiSystem->SameLine();
            if (_uiSystem->Checkbox("IsActive##" + name, &isActive))
                systemPair.second->SetActive(isActive);
        }


        if (isOpen)
        {
            auto visualiser = _customSystemVisualisers.find(systemPair.first);
            if (visualiser != _customSystemVisualisers.end())
                visualiser->second(_uiSystem, systemPair.second.get());

            _uiSystem->EndTreeNode();
        }
    }
}

void EngineInspectorSystem::DrawSceneUI()
{
    if (_uiSystem->BeginTreeNode("All Entites"))
    {
        for (auto entity : _entityManager->GetAllEntities())
        {
            std::string delLable = "Del##" + std::to_string(entity->GetId());
            if (_uiSystem->Button(delLable))
            {
                entity->Destroy();
            }
            _uiSystem->SameLine();
            _uiSystem->Text(entity->GetName());
            _uiSystem->SameLine();
            _uiSystem->Text(entity->GetTag().GetName());
        }
        _uiSystem->EndTreeNode();
    }

    if (_uiSystem->BeginTreeNode("Taged Entites"))
    {
        for (auto& tagPair : Core::ECS::TagManager::GetInstance().GetAllTags())
        {
            if (!_uiSystem->BeginTreeNode(tagPair.second))
                continue;

            for (auto entity : _entityManager->GetEntitiesWithTag(Core::ECS::Tag(tagPair.first)))
            {
                std::string delLable = "Del##" + std::to_string(entity->GetId());
                if (_uiSystem->Button(delLable))
                {
                    entity->Destroy();
                }
                _uiSystem->SameLine();
                _uiSystem->Text(entity->GetName());
            }
            _uiSystem->EndTreeNode();
        }
        _uiSystem->EndTreeNode();
    }
}

void EngineInspectorSystem::DrawInspectorUI()
{
    auto& entities = _entityManager->GetAllEntities();

    if (entities.size() < 1)
    {
        _uiSystem->Text("No entities");
        return;
    }

    if (_selectedEntity >= entities.size())
        _selectedEntity = entities.size() - 1;

    std::vector<std::string> componentNames;
    for (auto& entity : entities)
    {
        componentNames.push_back(entity->GetName());

        Core::TransformComponent* transform = nullptr;
        if (entity->QueryComponents(transform))
        {
            _uiSystem->AddWorldText(transform->Position, entity->GetName(), {1.0f, 1.0f, 0.0f},
                                    Render::Alignment::HorizontalCenter | Render::Alignment::VerticalCenter);
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
        const auto& componentType = typeid(*component);
        const auto& name = GetOrCreateSimplifiedName(componentType);
        bool isOpen = _uiSystem->BeginTreeNode(name, static_cast<int>(Render::TreeNode::DefaultOpen));
        bool isActive = component->IsActive();
        _uiSystem->SameLine();
        if (_uiSystem->Checkbox("IsActive##" + name, &isActive))
            component->SetActive(isActive);

        if (isOpen)
        {
            auto visualiser = _customComponentVisualisers.find(componentType);
            if (visualiser != _customComponentVisualisers.end())
            {
                visualiser->second(_uiSystem, component);
            }
            _uiSystem->EndTreeNode();
        }
    }
}

void EngineInspectorSystem::PuaseAllSystems(bool pause)
{
    _allSystemsPuased = pause;
    for (auto& systems : _engine->GetSystemManager()->GetSortedSystems())
    {
        for (auto& system : systems)
        {
            if (system->IsPermanent())
                continue;

            system->SetActive(!pause);
        }
    }
}

} // namespace Snowglobe::Engine