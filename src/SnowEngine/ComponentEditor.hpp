#pragma once

#include "UISystem.hpp"

#include "ECS/Component.hpp"

#include "TransformComponent.hpp"
#include "Physics2DComponent.hpp"
#include "Collider2DComponent.hpp"
#include "MeshComponent.hpp"


namespace Snowglobe::SnowEngine
{
    
class ComponentEditor
{
public:
    ComponentEditor(Render::UISystem& uiSystem) : _uiSystem(uiSystem) {}
    virtual void DrawUI(SnowCore::ECS::Component* component) = 0;

protected:
    Render::UISystem& _uiSystem;
};

template <class TComponent>
class TemplateComponentEditor : public ComponentEditor
{
public:
    TemplateComponentEditor(Render::UISystem& uiSystem) : ComponentEditor(uiSystem) {}
    
    void DrawUI(SnowCore::ECS::Component* component) override { DrawUI(static_cast<TComponent*>(component)); }

protected:
    virtual void DrawUI(TComponent* component) = 0;    
};

class TransformComponentEditor : public TemplateComponentEditor<SnowCore::TransformComponent>
{
public:
    TransformComponentEditor(Render::UISystem& uiSystem) : TemplateComponentEditor(uiSystem) {}
    
    void DrawUI(SnowCore::TransformComponent* component) override
    {
        _uiSystem.Text("Transform");
        _uiSystem.Drag("Position", &component->Position, 0.1f);
        _uiSystem.Drag("Rotation", &component->Rotation, 0.1f);
        _uiSystem.Drag("Scale", &component->Scale, 0.1f, 0.001f, 100.0f);
    }
};

class Physics2DComponentEditor : public TemplateComponentEditor<Physics2DComponent>
{
public:
    Physics2DComponentEditor(Render::UISystem& uiSystem) : TemplateComponentEditor(uiSystem) {}
    
    void DrawUI(Physics2DComponent* component) override
    {
        _uiSystem.Text("Physics2D");
        _uiSystem.Drag("Velocity", &component->Velocity, 0.1f);
        _uiSystem.Drag("AngularVelocity", &component->AngularVelocity, 0.1f);
        _uiSystem.Drag("Mass", &component->Mass, 0.1f, 0.001f, 100.0f);
    }
};

class Collider2DComponentEditor : public TemplateComponentEditor<Collider2DComponent>
{
public:
    Collider2DComponentEditor(Render::UISystem& uiSystem) : TemplateComponentEditor(uiSystem) {}
    
    void DrawUI(Collider2DComponent* component) override
    {
        _uiSystem.Text("Collider2D");
    }
};
    
class BaseComponentMaterialEditor : public TemplateComponentEditor<BaseComponentMaterial>
{
public:
    BaseComponentMaterialEditor(Render::UISystem& uiSystem) : TemplateComponentEditor(uiSystem) {}
    
    void DrawUI(BaseComponentMaterial* component) override
    {
        _uiSystem.Text("BaseComponentMaterial");
        auto material = component->GetMaterial();
        _uiSystem.Color("Color: ", &material->color);
    }
};

class DebugComponent : public SnowCore::ECS::Component
{
public:
    DebugComponent() = default;
    DebugComponent(bool drawEntityName) : DrawEntityName(drawEntityName) {}

    bool DrawEntityName = true;
};

class DebugComponentEditor : public TemplateComponentEditor<DebugComponent>
{
public:
    DebugComponentEditor(Render::UISystem& uiSystem) : TemplateComponentEditor(uiSystem) {}
    
    void DrawUI(DebugComponent* component) override
    {
        _uiSystem.Text("DebugComponent");
        _uiSystem.Checkbox("DrawEntityName", &component->DrawEntityName);
    }
};
    

} // namespace Snowglobe::SnowEngine
