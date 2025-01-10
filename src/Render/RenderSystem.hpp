#pragma once

#include <memory>

#include <ISnowSystem.hpp>
#include <Window.hpp>
#include <Camera.hpp>

namespace Snowglobe::Render
{
    class RenderSystem : public SnowCore::ISnowSystem
    {
    protected:
        std::unique_ptr<Window> _mainWindow;

        Camera _camera;
        
    public:
        virtual ~RenderSystem() override {}

        virtual void Update() override = 0;

        virtual void InitializeWindow(const WindowParams& params) = 0;
        virtual void InitializeRenderScene() = 0;

        Window* GetMainWindow() const { return _mainWindow.get(); }
        Camera& GetCamera() { return _camera; }
    };
    
} // namespace Snowglobe::Render
