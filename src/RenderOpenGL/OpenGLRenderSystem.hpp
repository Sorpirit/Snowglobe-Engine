#pragma once

#include <memory>

#include <RenderSystem.hpp>
#include <Window.hpp>

#include <ShaderCompiler.hpp>

#include <SnowFileSystem.hpp>
#include "PositionVertex.hpp"
#include "Mesh.hpp"
#include "RenderEntity.hpp"
#include "SimpleShapeMaterial.hpp"
#include "SceneParameters.hpp"

namespace Snowglobe::RenderOpenGL
{
    class OpenGLRenderSystem : public Render::RenderSystem
    {
    public:
        std::unique_ptr<ShaderCompiler> _shaderCompiler;

        OpenGLRenderSystem();
        virtual ~OpenGLRenderSystem() override;

        virtual void Update() override;
        virtual void InitializeWindow(const Render::WindowParams& params) override;
        virtual void InitializeRenderScene() override;

    private:
        SceneParameters _sceneParameters;

        std::unique_ptr<Mesh<PositionVertex>> _mesh;
        RenderEntity _entity;
        SimpleShapeMaterial _material;
        
        unsigned int _shaderProgram;
    };    
} // namespace Snowglobe::RenderOpenGL
