#include "ComponentEditorSystem.hpp"
#include "Engine.hpp"

#include <string>

namespace Snowglobe::Engine
{

void ComponentEditorSystem::Update()
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

void ComponentEditorSystem::DrawSystemUI()
{
    bool isActive = true;

    std::string buttonText = _allSystemsPuased ? "Unpause" : "Pause";
    if (_uiSystem->Button(buttonText))
    {
        PuaseAllSystems(!_allSystemsPuased);
    }

    for (auto& systemPair : _engine->GetSystemManager()->GetTypedSystems())
    {
        // skip "class Snowglobe::"
        const int skipChars = 17;
        if (_uiSystem->BeginTreeNode((systemPair.first.name() + skipChars)))
        {
            if (!systemPair.second->IsPermanent())
            {
                isActive = systemPair.second->IsActive();
                if (_uiSystem->Checkbox("IsActive", &isActive))
                {
                    systemPair.second->SetActive(isActive);
                }
            }

            _uiSystem->EndTreeNode();
        }
    }
}

void ComponentEditorSystem::DrawSceneUI()
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

void ComponentEditorSystem::DrawInspectorUI()
{
    auto& entities = _entityManager->GetAllEntities();

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
        auto visualiser = _visualisers.find(typeid(*component));
        if (visualiser != _visualisers.end())
        {
            visualiser->second->DrawUI(component);
        }
    }
}

void ComponentEditorSystem::PuaseAllSystems(bool pause)
{
    _allSystemsPuased = pause;
    for (auto& systemPair : _engine->GetSystemManager()->GetTypedSystems())
    {
        if (systemPair.second->IsPermanent())
            continue;

        systemPair.second->SetActive(!pause);
    }
}

} // namespace Snowglobe::Engine