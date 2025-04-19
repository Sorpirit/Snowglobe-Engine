#pragma once
#include "Collider2DComponent.hpp"
#include "EngineInspectorSystem.hpp"
#include "LightsManager.hpp"
#include "MeshComponent.hpp"
#include "NEdgeShape2DComponent.hpp"
#include "Physics2DComponent.hpp"
#include "TransformComponent.hpp"
namespace Snowglobe::Engine
{
class CommonVisualizers
{
  public:
    static void RegisterVisualizers(EngineInspectorSystem* inspector)
    {
        inspector->RegisterComponentVisualiser<Core::TransformComponent>(
            [](Render::UISystem& ui, Core::TransformComponent& component) {
                ui.Drag("Position", &component.Position, 0.1f);
                ui.Drag("Rotation", &component.Rotation, 0.1f);
                ui.Drag("Scale", &component.Scale, 0.1f, 0.001f, 100.0f);
            });

        inspector->RegisterComponentVisualiser<Physics2DComponent>(
            [](Render::UISystem& ui, Physics2DComponent& component) {
                ui.Drag("Velocity", &component.Velocity, 0.1f);
                ui.Drag("AngularVelocity", &component.AngularVelocity, 0.1f);
                ui.Drag("Mass", &component.Mass, 0.1f, 0.001f, 100.0f);
                ui.Drag("Drag", &component.Drag, 0.01f, 0.0f, 1.0f);
                ui.Slider("GravityScale", &component.GravityScale, 0.0f, 1.0f);
                ui.Slider("Bounciness", &component.Bounciness, 0.01f, 1.0f);
            });

        inspector->RegisterComponentVisualiser<Collider2DComponent>(
            [](Render::UISystem& ui, Collider2DComponent& component) {
                ui.Checkbox("IsTrigger", &component.IsTrigger);
                size_t index = static_cast<size_t>(component.ShapeType);
                std::string options[] = {"Circle", "AABB"};
                ui.Combo("ShapeType", &index, options, 2);
                component.ShapeType = static_cast<CollisionShapeType>(index);
            });

        inspector->RegisterComponentVisualiser<BaseComponentMaterial>(
            [](Render::UISystem& ui, BaseComponentMaterial& component) {
                auto material = component.GetMaterial();
                auto proxy = component.GetProxy();
                bool drawMesh = proxy->IsVisible();
                if (ui.Checkbox("DrawMesh: ", &drawMesh))
                {
                    proxy->SetVisible(drawMesh);
                }
                ui.Color("Color: ", &material->color);
            });

        inspector->RegisterComponentVisualiser<Render::NEdgeShape2DComponent>(
            [](Render::UISystem& ui, Render::NEdgeShape2DComponent& component) {
                ui.Slider("EdgeCount", &component.EdgeCount, 3, 32);
                ui.Color("Color", &component.Color);
            });

        inspector->RegisterComponentVisualiser<RenderOpenGL::DirectionalLightComponent>(
            [](Render::UISystem& ui, RenderOpenGL::DirectionalLightComponent& component) {
                ui.Color("Color", &component.Light.LightColor);
                ui.Slider("AmbientIntensity", &component.Light.AmbientIntensity, 0.0f, 1.0f);
                ui.Slider("UseEnvMapping", &component.Light.UseEnvMapping, 0, 1);
                ui.Slider("EnvIntensity", &component.Light.EnvIntensity, 0, 1);
            });

        inspector->RegisterComponentVisualiser<RenderOpenGL::PointLightComponent>(
            [](Render::UISystem& ui, RenderOpenGL::PointLightComponent& component) {
                ui.Color("Color", &component.Light.LightColor);
                ui.Slider("AmbientIntensity", &component.Light.AmbientIntensity, 0.0f, 1.0f);
                ui.Slider("AttenuationCoefficients", &component.Light.AttenuationCoefficients, 0.0f, 1.0f);
                ui.Slider("MaxDistance", &component.Light.MaxDistance, 0.05f, 100.0f);
            });

        inspector->RegisterComponentVisualiser<RenderOpenGL::SpotLightComponent>(
            [](Render::UISystem& ui, RenderOpenGL::SpotLightComponent& component) {
                ui.Color("Color", &component.Light.LightColor);
                ui.Slider("AmbientIntensity", &component.Light.AmbientIntensity, 0.0f, 1.0f);
                ui.SliderAngle("InnerCutoffAngle", &component.Light.InnerCutoffAngle, 0.0f, 360.0f);
                ui.SliderAngle("OuterCutoffAngle", &component.Light.OuterCutoffAngle, 0.0f, 360.0f);
            });
    }
};
} // namespace Snowglobe::Engine
