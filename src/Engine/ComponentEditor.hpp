#pragma once

#include "UISystem.hpp"

#include "ECS/Component.hpp"

#include "TransformComponent.hpp"
#include "Physics2DComponent.hpp"
#include "Collider2DComponent.hpp"
#include "MeshComponent.hpp"
#include "NEdgeShape2DComponent.hpp"

namespace Snowglobe::Engine
{
    
class ComponentEditor
{
public:
    virtual ~ComponentEditor() = default;
    ComponentEditor(Render::UISystem* uiSystem) : _uiSystem(uiSystem) {}
    
    virtual void DrawUI(Core::ECS::Component* component) = 0;
    virtual std::type_index GetComponentRuntimeType() const = 0;
    
    
protected:
    Render::UISystem* _uiSystem;
};

template <class TComponent>
class TemplateComponentEditor : public ComponentEditor
{
    static_assert(std::is_base_of_v<Core::ECS::Component, TComponent>, "TComponent must derive from Component");
    
public:
    TemplateComponentEditor(Render::UISystem* uiSystem) : ComponentEditor(uiSystem), _componentType(typeid(TComponent)) {}
    
    void DrawUI(Core::ECS::Component* component) override { DrawUITemplate(static_cast<TComponent*>(component)); }
    std::type_index GetComponentRuntimeType() const override { return _componentType; }
protected:
    virtual void DrawUITemplate(TComponent* component) = 0;

private:
    std::type_index _componentType;
};

class TransformComponentEditor : public TemplateComponentEditor<Core::TransformComponent>
{
public:
    TransformComponentEditor(Render::UISystem* uiSystem) : TemplateComponentEditor(uiSystem) {}
    
    void DrawUITemplate(Core::TransformComponent* component) override
    {
        _uiSystem->Text("Transform");
        _uiSystem->Drag("Position", &component->Position, 0.1f);
        _uiSystem->Drag("Rotation", &component->Rotation, 0.1f);
        _uiSystem->Drag("Scale", &component->Scale, 0.1f, 0.001f, 100.0f);
    }
};

class Physics2DComponentEditor : public TemplateComponentEditor<Physics2DComponent>
{
public:
    Physics2DComponentEditor(Render::UISystem* uiSystem) : TemplateComponentEditor(uiSystem) {}
    
    void DrawUITemplate(Physics2DComponent* component) override
    {
        _uiSystem->Text("Physics2D");
        _uiSystem->Drag("Velocity", &component->Velocity, 0.1f);
        _uiSystem->Drag("AngularVelocity", &component->AngularVelocity, 0.1f);
        _uiSystem->Drag("Mass", &component->Mass, 0.1f, 0.001f, 100.0f);
        _uiSystem->Drag("Drag", &component->Drag, 0.01f, 0.0f, 1.0f);
        _uiSystem->Slider("Bouncines", &component->Bouncines, 0.01f, 1.0f);
    }
};

class Collider2DComponentEditor : public TemplateComponentEditor<Collider2DComponent>
{
public:
    Collider2DComponentEditor(Render::UISystem* uiSystem) : TemplateComponentEditor(uiSystem) {}
    
    void DrawUITemplate(Collider2DComponent* component) override
    {
        _uiSystem->Text("Collider2D");
    }
};
    
class BaseComponentMaterialEditor : public TemplateComponentEditor<BaseComponentMaterial>
{
public:
    BaseComponentMaterialEditor(Render::UISystem* uiSystem) : TemplateComponentEditor(uiSystem) {}
    
    void DrawUITemplate(BaseComponentMaterial* component) override
    {
        _uiSystem->Text("BaseComponentMaterial");
        auto material = component->GetMaterial();
        auto proxy = component->GetProxy();
        bool drawMesh = proxy->IsVisible();
        if (_uiSystem->Checkbox("DrawMesh: ", &drawMesh))
        {
            proxy->SetVisible(drawMesh);
        }
        _uiSystem->Color("Color: ", &material->color);
    }
};

class NEdgeShape2DComponentEditor : public TemplateComponentEditor<Render::NEdgeShape2DComponent>
{
public:
    NEdgeShape2DComponentEditor(Render::UISystem* uiSystem) : TemplateComponentEditor(uiSystem) {}

    void DrawUITemplate(Render::NEdgeShape2DComponent* component) override
    {
        _uiSystem->Text("NEdgeShape2DComponent");
        _uiSystem->Slider("EdgeCount", &component->EdgeCount, 3, 32);
        _uiSystem->Color("Color", &component->Color);
    }
};
    
}
