#include "ImguiSystem.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <Window.hpp>
#include <GLFW/WindowGLFW.hpp>

#include "glm/glm.hpp"
#include "OpenGLRenderSystem.hpp"

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

        auto glfwWindow = dynamic_cast<Render::GLFW::WindowGLFW*>(window);

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

    void ImguiSystem::UpdateEarly()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        if (_drawWorldText)
        {
            static ImGuiWindowFlags flags = 
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize | 
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoNav |
            ImGuiWindowFlags_NoDecoration | 
            ImGuiWindowFlags_NoInputs | 
            ImGuiWindowFlags_NoBackground;

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowBgAlpha(0.0f);
            if(ImGui::Begin("Window", &_drawWorldText, flags))
            {
                const Render::Camera& camera = OpenGLRenderSystem::GetInstance()->GetCamera();

                for (const auto& text: _worldTexts)
                {
                    glm::vec2 screenPos = camera.WorldToScreen(text.position);
                    auto textSize = ImGui::CalcTextSize(text.text.c_str());

                    if (text.alignment & Render::Alignment::HorizontalCenter)
                        screenPos.x -= textSize.x / 2.0f;
                    else if (text.alignment & Render::Alignment::HorizontalRight)
                        screenPos.x -= textSize.x;

                    if (text.alignment & Render::Alignment::VerticalCenter)
                        screenPos.y -= textSize.y / 2.0f;
                    else if (text.alignment & Render::Alignment::VerticalBottom)
                        screenPos.y -= textSize.y;

                    if (_defaultFont)
                    {
                        ImGui::GetForegroundDrawList()->AddText(_defaultFont, 16.0f,ImVec2(screenPos.x, screenPos.y), ImColor(text.color.r, text.color.g, text.color.b), text.text.c_str());
                    }
                    else
                    {
                        ImGui::GetForegroundDrawList()->AddText(ImVec2(screenPos.x, screenPos.y), ImColor(text.color.r, text.color.g, text.color.b), text.text.c_str());
                    }
                    
                }
                ImGui::End();
            }
            
            _worldTexts.clear();
        }
        
    }

    void ImguiSystem::UpdateLate()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    Render::UIPanel ImguiSystem::OpenUIPanel(const std::string& title)
    {
        ImGui::Begin(title.c_str());
        return {this};
    }

    void ImguiSystem::Close(Render::UIPanel *panel)
    {
        if(!panel->Close())
            return;

        ImGui::End();
    }

    void ImguiSystem::SetDefaultFont(Core::SnowFileHandle fontFileHandle)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto path = fontFileHandle.GetFullPath();
        _defaultFont = io.Fonts->AddFontFromFileTTF(path.string().c_str(), 16.0f);
    }

    void ImguiSystem::Text(const std::string& text)
    {
        ImGui::TextUnformatted(text.c_str());
    }

    void ImguiSystem::Text(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        ImGui::TextV(fmt, args);
        va_end(args);
    }

    bool ImguiSystem::Checkbox(const std::string& label, bool* value)
    {
        return ImGui::Checkbox(label.c_str(), value);
    }

    void ImguiSystem::Slider(const std::string& label, float* value, float min, float max, const std::string& format,
        int flags)
    {
        ImGui::SliderFloat(label.c_str(), value, min, max, format.c_str(), flags);
    }

    void ImguiSystem::Slider(const std::string& label, glm::vec2* value, float min, float max,
        const std::string& format, int flags)
    {
        ImGui::SliderFloat2(label.c_str(), &value->x, min, max, format.c_str(), flags);
    }

    void ImguiSystem::Slider(const std::string& label, glm::vec3* value, float min, float max,
        const std::string& format, int flags)
    {
        ImGui::SliderFloat3(label.c_str(), &value->x, min, max, format.c_str(), flags);
    }

    void ImguiSystem::Slider(const std::string& label, glm::vec4* value, float min, float max,
        const std::string& format, int flags)
    {
        ImGui::SliderFloat4(label.c_str(), &value->x, min, max, format.c_str(), flags);
    }

    void ImguiSystem::Slider(const std::string& label, int* value, int min, int max, const std::string& format,
        int flags)
    {
        ImGui::SliderInt(label.c_str(), value, min, max, format.c_str(), flags);
    }

    void ImguiSystem::Slider(const std::string& label, glm::ivec2* value, int min, int max,
        const std::string& format, int flags)
    {
        ImGui::SliderInt2(label.c_str(), &value->x, min, max, format.c_str(), flags);
    }

    void ImguiSystem::Slider(const std::string& label, glm::ivec3* value, int min, int max,
        const std::string& format, int flags)
    {
        ImGui::SliderInt3(label.c_str(), &value->x, min, max, format.c_str(), flags);
    }

    void ImguiSystem::Slider(const std::string& label, glm::ivec4* value, int min, int max,
        const std::string& format, int flags)
    {
        ImGui::SliderInt4(label.c_str(), &value->x, min, max, format.c_str(), flags);
    }

    void ImguiSystem::SliderAngle(const std::string& label, float* value, float min, float max,
        const std::string& format, int flags)
    {
        ImGui::SliderAngle(label.c_str(), value, min, max, format.c_str(), flags);
    }

    void ImguiSystem::Color(const std::string& label, glm::vec3* value, int flags)
    {
        ImGui::ColorEdit3(label.c_str(), &value->x, flags);
    }

    void ImguiSystem::Color(const std::string& label, glm::vec4* value, int flags)
    {
        ImGui::ColorEdit4(label.c_str(), &value->x, flags);
    }

    void ImguiSystem::Combo(const std::string& label, size_t* current, const std::string items[], size_t itemsCount, int flags)
    {
        if (ImGui::BeginCombo(label.c_str(), items[*current].c_str(), flags))
        {
            for (size_t n = 0; n < itemsCount; n++)
            {
                const bool is_selected = (*current == n);
                if (ImGui::Selectable(items[n].c_str(), is_selected))
                    *current = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    void ImguiSystem::Combo(const std::string& label, size_t* current, const std::vector<std::string>& items, int flags)
    {
        if (ImGui::BeginCombo(label.c_str(), items[*current].c_str(), flags))
        {
            for (size_t n = 0; n < items.size(); n++)
            {
                const bool is_selected = (*current == n);
                if (ImGui::Selectable(items[n].c_str(), is_selected))
                    *current = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    bool ImguiSystem::BeginTreeNode(const std::string& label, int flags)
    {
        return ImGui::TreeNode(label.c_str());
    }

    void ImguiSystem::EndTreeNode()
    {
        ImGui::TreePop();
    }

    bool ImguiSystem::BeginTabBar(const std::string& label, int flags)
    {
        return ImGui::BeginTabBar(label.c_str(), flags);
    }

    void ImguiSystem::EndTabBar()
    {
        ImGui::EndTabBar();
    }

    bool ImguiSystem::BeginTabBarItem(const std::string& label, int flags)
    {
        return ImGui::BeginTabItem(label.c_str(), 0, flags);
    }

    void ImguiSystem::EndTabBarItem()
    {
        ImGui::EndTabItem();
    }

    void ImguiSystem::Separator(int flags)
    {
        ImGui::Separator();
    }

    void ImguiSystem::SameLine()
    {
        ImGui::SameLine();
    }

    void ImguiSystem::ToolTip(const std::string& text)
    {
        ImGui::SetItemTooltip("%s", text.c_str());
    }

    bool ImguiSystem::Button(const std::string& label)
    {
        return ImGui::Button(label.c_str());
    }

    bool ImguiSystem::Input(const std::string& label, char* textBuff, size_t size, int flags)
    {
        return ImGui::InputText(label.c_str(), textBuff, size, flags);
    }

    bool ImguiSystem::Input(const std::string& label, float* value, float step, float stepFast,
        const std::string& format, int flags)
    {
        return ImGui::InputFloat(label.c_str(), value, step, stepFast, format.c_str(), flags);
    }

    bool ImguiSystem::Input(const std::string& label, glm::vec2* value, const std::string& format, int flags)
    {
        return ImGui::InputFloat2(label.c_str(), &value->x, format.c_str(), flags);
    }

    bool ImguiSystem::Input(const std::string& label, glm::vec3* value, const std::string& format, int flags)
    {
        return ImGui::InputFloat3(label.c_str(), &value->x, format.c_str(), flags);
    }

    bool ImguiSystem::Input(const std::string& label, glm::vec4* value, const std::string& format, int flags)
    {
        return ImGui::InputFloat4(label.c_str(), &value->x, format.c_str(), flags);
    }

    bool ImguiSystem::Input(const std::string& label, int* value, int step, int stepFast,
        int flags)
    {
        return ImGui::InputInt(label.c_str(), value, step, stepFast, flags);
    }

    bool ImguiSystem::Input(const std::string& label, glm::ivec2* value, int flags)
    {
        return ImGui::InputInt2(label.c_str(), &value->x, flags);
    }

    bool ImguiSystem::Input(const std::string& label, glm::ivec3* value, int flags)
    {
        return ImGui::InputInt3(label.c_str(), &value->x, flags);
    }

    bool ImguiSystem::Input(const std::string& label, glm::ivec4* value, int flags)
    {
        return ImGui::InputInt3(label.c_str(), &value->x, flags);
    }

    bool ImguiSystem::Drag(const std::string& label, float* value, float speed, float min, float max,
        const std::string& format, int flags)
    {
        return ImGui::DragFloat(label.c_str(), value, speed, min, max, format.c_str(), flags);
    }

    bool ImguiSystem::Drag(const std::string& label, glm::vec2* value, float speed, float min, float max,
        const std::string& format, int flags)
    {
        return ImGui::DragFloat2(label.c_str(), &value->x, speed, min, max, format.c_str(), flags);
    }

    bool ImguiSystem::Drag(const std::string& label, glm::vec3* value, float speed, float min, float max,
        const std::string& format, int flags)
    {
        return ImGui::DragFloat3(label.c_str(), &value->x, speed, min, max, format.c_str(), flags);
    }

    bool ImguiSystem::Drag(const std::string& label, glm::vec4* value, float speed, float min, float max,
        const std::string& format, int flags)
    {
        return ImGui::DragFloat4(label.c_str(), &value->x, speed, min, max, format.c_str(), flags);
    }

    bool ImguiSystem::Drag(const std::string& label, int* value, float speed, int min, int max,
        const std::string& format, int flags)
    {
        return ImGui::DragInt(label.c_str(), value, speed, min, max, format.c_str(), flags);
    }

    bool ImguiSystem::Drag(const std::string& label, glm::ivec2* value, float speed, int min, int max,
        const std::string& format, int flags)
    {
        return ImGui::DragInt2(label.c_str(), &value->x, speed, min, max, format.c_str(), flags);
    }

    bool ImguiSystem::Drag(const std::string& label, glm::ivec3* value, float speed, int min, int max,
        const std::string& format, int flags)
    {
        return ImGui::DragInt3(label.c_str(), &value->x, speed, min, max, format.c_str(), flags);
    }

    bool ImguiSystem::Drag(const std::string& label, glm::ivec4* value, float speed, int min, int max,
        const std::string& format, int flags)
    {
        return ImGui::DragInt4(label.c_str(), &value->x, speed, min, max, format.c_str(), flags);
    }
} // namespace Snowglobe::Render


