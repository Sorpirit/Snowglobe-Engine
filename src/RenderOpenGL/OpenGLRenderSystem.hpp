#pragma once

#include <memory>

#include <RenderSystem.hpp>
#include <Window.hpp>

#include <ShaderCompiler.hpp>

#include <SnowFileSystem.hpp>
#include "PositionColorVertex.hpp"
#include "Mesh.hpp"

namespace Snowglobe::RenderOpenGL
{
    class OpenGLRenderSystem : public Render::RenderSystem
    {
    public:
        std::unique_ptr<ShaderCompiler> _shaderCompiler;

        OpenGLRenderSystem(/* args */);
        virtual ~OpenGLRenderSystem() override;

        virtual void Update() override;
        virtual void InitializeWindow(const Render::WindowParams& params) override;
        virtual void InitializeRenderScene() override;

    private:
        std::unique_ptr<Mesh<PositionColorVertex>> _mesh;
        unsigned int _shaderProgram;
    };    
} // namespace Snowglobe::RenderOpenGL
