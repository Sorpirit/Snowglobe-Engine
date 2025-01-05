#pragma once

#include <RenderSystem.hpp>
#include <Window.hpp>

namespace Snowglobe::RenderOpenGL
{
    class OpenGLRenderSystem : public Render::RenderSystem
    {
    private:
        /* data */
    public:
        OpenGLRenderSystem(/* args */);
        virtual ~OpenGLRenderSystem() override;

        virtual void Update() override;
        virtual void InitializeWindow(const Render::WindowParams& params) override;
    };    
} // namespace Snowglobe::RenderOpenGL
