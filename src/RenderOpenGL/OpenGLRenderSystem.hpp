#pragma once

#include <memory>
#include <vector>
#include <list>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include <RenderSystem.hpp>
#include <Window.hpp>

#include <ShaderCompiler.hpp>

#include <SnowFileSystem.hpp>
#include "RenderEntity.hpp"
#include "SceneParameters.hpp"

#include "VertexBufferPtrOpenGL.hpp"
#include "IndexBufferPtrOpenGL.hpp"
#include "VertexLayoutDescriptor.hpp"
#include "MeshOpenGL.hpp"

#include "Materials/MaterialBaseOpenGL.hpp"
#include "Materials/IMaterialManager.hpp"

#include "Materials/TemplateMaterialManager.hpp"

#include "RenderPass.hpp"

namespace Snowglobe::RenderOpenGL
{
    class OpenGLRenderSystem : public Render::RenderSystem
    {
    public:
        OpenGLRenderSystem();
        ~OpenGLRenderSystem() override;

        static OpenGLRenderSystem* GetInstance() { return _instance; }

        void Update() override;
        void InitializeWindow(const Render::WindowParams& params) override;
        void InitializeRenderScene() override;
        
        Render::IndexBufferPtr* AllocateIndexBufferPtr(const std::vector<uint32_t>& indices, const std::string& debugName = "IndexBuffer") override;

        Render::MeshProxy* CreateMeshProxy(const Render::VertexBufferPtr& vertexBuffer, const std::string& = "MeshProxy[Vertex]") override;
        Render::MeshProxy* CreateMeshProxy(const Render::VertexBufferPtr& vertexBuffer, const Render::IndexBufferPtr& indexBuffer, const std::string& = "MeshProxy[Vertex,Index]") override;

        ShaderCompiler* GetShaderCompiler() const { return _shaderCompiler.get(); }
        const Render::Camera& GetCamera() const { return _camera; }

    protected:
        Render::VertexBufferPtr* AllocateVertexBufferPtrImpl(std::type_index vertexType, size_t vertexCount, const void* bufferPtr, const std::string& debugName) override;
        Render::MaterialBase* CreateMaterialInstanceImpl(std::type_index materialType, const std::string& debugName) override;
        
    private:
        static OpenGLRenderSystem* _instance;

        std::unique_ptr<ShaderCompiler> _shaderCompiler;

        std::unordered_map<std::type_index, const VertexLayoutDescriptor*> _vertexLayoutDescriptors;
        std::list<VertexBufferPtrOpenGL> _vertexBuffers;
        std::list<IndexBufferPtrOpenGL> _indexBuffers;

        std::unordered_map<std::type_index, Materials::IMaterialManager*> _materialManages;

        Render::RenderID _nextMeshId = {1};
        std::list<MeshOpenGL> _meshes;

        std::unordered_map<RenderPassSignature, std::unique_ptr<RenderPass>> _renderPasses;

        template <typename MaterialImpl, typename MaterialData>
        void RegisterMaterialManager()
        {
            _materialManages.insert({typeid(MaterialData), Materials::TemplateMaterialManager<MaterialImpl, MaterialData>::GetInstance()});
        }
    };    

    
} // namespace Snowglobe::RenderOpenGL
