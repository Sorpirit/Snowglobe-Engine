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
};

} // namespace Snowglobe::RenderVulkan
