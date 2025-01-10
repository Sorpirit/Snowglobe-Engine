#include "ImguiSystem.hpp"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <Window.hpp>
#include <GLFW/WindowGLFW.hpp>

namespace Snowglobe::RenderOpenGL::Imgui
{
    ImguiSystem::ImguiSystem(Render::Window *window)
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        auto glfwWindow = static_cast<Render::GLFW::WindowGLFW*>(window);

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(glfwWindow->GetWindow(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
        ImGui_ImplOpenGL3_Init();
    }

    ImguiSystem::~ImguiSystem()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImguiSystem::EarlyUpdate()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(); // Show demo window! :)
    }

    void ImguiSystem::Update()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    std::unique_ptr<Render::UIPanel> ImguiSystem::OpenUIPanel(const char* title)
    {
        ImGui::Begin(title); 
        return std::unique_ptr<Render::UIPanel>();
    }

    void ImguiSystem::Close(Render::UIPanel *panel)
    {
        if(!panel->Close())
            return;

        ImGui::End();
    }

} // namespace Snowglobe::Render


