#pragma once

#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <vector>

#include <ECS/ISystem.hpp>

#include <Window.hpp>
#include <Camera.hpp>

#include "RenderProxy.hpp"
#include "MeshProxy.hpp"

#include "FileTexture.hpp"
#include "GizmosAPI.hpp"

#include "IndexBufferPtr.hpp"
#include "MaterialBase.hpp"
#include "Texture2DPtr.hpp"
#include "UISystem.hpp"
#include "VertexBufferPtr.hpp"

namespace Snowglobe::Render
{
    class RenderSystem : public Core::ECS::ISystem
    {
    public:
        RenderSystem() : ISystem(true) {}
        void Update() override = 0;

        virtual void InitializeWindow(const WindowParams& params) = 0;
        virtual void InitializeRenderScene() = 0;

        template <typename T>
        VertexBufferPtr* AllocateVertexBufferPtr(const std::vector<T>& vertices, const std::string& debugName = "VertexBuffer")
        {
            return AllocateVertexBufferPtrImpl(typeid(T),  vertices.size(),  static_cast<const void*>(vertices.data()), debugName);
        }

        virtual IndexBufferPtr* AllocateIndexBufferPtr(const std::vector<uint32_t>& indices, const std::string& debugName = "IndexBuffer") = 0;

        virtual Texture2DPtr CreateTexture2D(const Core::FileTexture& texture, const Texture2DDescriptor& desc, const std::string& debugName = "Texture2D") = 0;

        virtual MeshProxy* CreateMeshProxy(const VertexBufferPtr& vertexBuffer, const std::string& = "MeshProxy[Vertex]") = 0;
        virtual MeshProxy* CreateMeshProxy(const VertexBufferPtr& vertexBuffer, const IndexBufferPtr& indexBuffer, const std::string& = "MeshProxy[Vertex,Index]") = 0;

        template <typename T>
        MaterialInstance<T> CreateMaterialInstance(const std::string& debugName = "VertexBuffer")
        {
            return MaterialInstance<T>(CreateMaterialInstanceImpl(typeid(T), debugName));
        }

        Window* GetMainWindow() const { return _mainWindow.get(); }
        Camera& GetCamera() { return _camera; }
        UISystem* GetUISystem() const { return _uiSystem; }
        virtual GizmosAPI* GetGizmos() = 0;

        void SetUISystem(UISystem* uiSystem) { _uiSystem = uiSystem; }
    
    protected:
        virtual VertexBufferPtr* AllocateVertexBufferPtrImpl(std::type_index vertexType, size_t vertexCount,const  void* bufferPtr, const std::string& debugName) = 0;
        virtual MaterialBase* CreateMaterialInstanceImpl(std::type_index materialType, const std::string& debugName) = 0;

        std::unique_ptr<Window> _mainWindow;
        UISystem* _uiSystem = nullptr;
        
        Camera _camera;
    };
    
} // namespace Snowglobe::Render
