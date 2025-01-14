#pragma once

#include <RenderSystem.hpp>

namespace Snowglobe::RenderVulkan
{
    
class VulkanRenderingSystem : public Render::RenderSystem
{
public:
    VulkanRenderingSystem(/* args */);
    virtual ~VulkanRenderingSystem() override;

    virtual void Update() override;
    virtual void InitializeWindow(const Render::WindowParams& params) override;
    virtual void InitializeRenderScene() override;

    virtual Render::MeshProxy* CreateMeshProxy(const Render::RenderID& vertexSet, std::string debugName) override;

    virtual Render::RenderID AllocateVertexSet(const Render::IVertexSet& vertexSet) override;
    virtual void FreeVertexSet(const Render::RenderID& vertexSet) override;
};

} // namespace Snowglobe::RenderVulkan
