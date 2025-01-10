#pragma once

#include <memory>

#include <UISystem.hpp>
#include <Window.hpp>

namespace Snowglobe::RenderOpenGL::Imgui
{
    class ImguiSystem : public Render::UISystem, public SnowCore::ISnowFrameSystem
    {
    protected:
    public:
        ImguiSystem(Render::Window* window);
        virtual ~ImguiSystem() override;

        virtual void EarlyUpdate() override;
        virtual void Update() override;

        virtual std::unique_ptr<Render::UIPanel> OpenUIPanel(const char* title);
        virtual void Close(Render::UIPanel* panel);
    };
    
} // namespace Snowglobe::Render
