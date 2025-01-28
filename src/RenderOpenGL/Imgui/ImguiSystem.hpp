#pragma once

#include <memory>
#include <string>

#include <UISystem.hpp>
#include <Window.hpp>

#include "imgui.h"

namespace Snowglobe::RenderOpenGL::Imgui
{
    struct WorldText
    {
        std::string text;
        glm::vec3 position;
        glm::vec3 color;
        int alignment;
    };
    
    class ImguiSystem : public Render::UISystem
    {
    public:
        ImguiSystem(Render::Window* window);
        ~ImguiSystem();

        void UpdateEarly() override;
        void UpdateLate() override;

        Render::UIPanel OpenUIPanel(const std::string& title) override;
        void Close(Render::UIPanel* panel) override;
        
        void AddWorldText(const glm::vec3& position, const std::string& text, const glm::vec3& color = {1.0f, 1.0f, 0.0f}, int alignment = 0) override
        {
            if (!_drawWorldText)
                return;
            _worldTexts.emplace_back(text, position, color, alignment);
        }
        
        void SetDefaultFont(Core::SnowFileHandle fontFileHandle) override;

        void Text(const std::string& text) override;
        void Text(const char* fmt, ...) override;
        
        bool Checkbox(const std::string& label, bool* value) override;

        bool Input(const std::string& label, char* textBuff, size_t size, int flags = 0) override;
        bool Input(const std::string& label, float* value, float step, float stepFast, const std::string& format = "%.3f", int flags = 0) override;
        bool Input(const std::string& label, glm::vec2* value, const std::string& format = "%.3f", int flags = 0) override;
        bool Input(const std::string& label, glm::vec3* value, const std::string& format = "%.3f", int flags = 0) override;
        bool Input(const std::string& label, glm::vec4* value, const std::string& format = "%.3f", int flags = 0) override;
        bool Input(const std::string& label, int* value, int step, int stepFast, int flags = 0) override;
        bool Input(const std::string& label, glm::ivec2* value, int flags = 0) override;
        bool Input(const std::string& label, glm::ivec3* value, int flags = 0) override;
        bool Input(const std::string& label, glm::ivec4* value, int flags = 0) override;

        bool Drag(const std::string& label, float* value, float speed = 0, float min = 0, float max = 0, const std::string& format = "%.3f", int flags = 0) override;
        bool Drag(const std::string& label, glm::vec2* value, float speed = 0, float min = 0, float max = 0, const std::string& format = "%.3f", int flags = 0) override;
        bool Drag(const std::string& label, glm::vec3* value, float speed = 0, float min = 0, float max = 0, const std::string& format = "%.3f", int flags = 0) override;
        bool Drag(const std::string& label, glm::vec4* value, float speed = 0, float min = 0, float max = 0, const std::string& format = "%.3f", int flags = 0) override;
        bool Drag(const std::string& label, int* value, float speed = 0, int min = 0, int max = 0, const std::string& format = "%d", int flags = 0) override;
        bool Drag(const std::string& label, glm::ivec2* value, float speed = 0, int min = 0, int max = 0, const std::string& format = "%d", int flags = 0) override;
        bool Drag(const std::string& label, glm::ivec3* value, float speed = 0, int min = 0, int max = 0, const std::string& format = "%d", int flags = 0) override;
        bool Drag(const std::string& label, glm::ivec4* value, float speed = 0, int min = 0, int max = 0, const std::string& format = "%d", int flags = 0) override;
        
        void Slider(const std::string& label, float* value, float min, float max, const std::string& format = "%.3f", int flags = Render::Slider::None) override;
        void Slider(const std::string& label, glm::vec2* value, float min, float max, const std::string& format = "%.3f", int flags = Render::Slider::None) override;
        void Slider(const std::string& label, glm::vec3* value, float min, float max, const std::string& format = "%.3f", int flags = Render::Slider::None) override;
        void Slider(const std::string& label, glm::vec4* value, float min, float max, const std::string& format = "%.3f", int flags = Render::Slider::None) override;
        void Slider(const std::string& label, int* value, int min, int max, const std::string& format = "%d", int flags = Render::Slider::None) override;
        void Slider(const std::string& label, glm::ivec2* value, int min, int max, const std::string& format = "%d", int flags = Render::Slider::None) override;
        void Slider(const std::string& label, glm::ivec3* value, int min, int max, const std::string& format = "%d", int flags = Render::Slider::None) override;
        void Slider(const std::string& label, glm::ivec4* value, int min, int max, const std::string& format = "%d", int flags = Render::Slider::None) override;
        void SliderAngle(const std::string& label, float* value, float min, float max, const std::string& format = "%.3f", int flags = Render::Slider::None) override;

        void Color(const std::string& label, glm::vec3* value, int flags = 0) override;
        void Color(const std::string& label, glm::vec4* value, int flags = 0) override;

        void Combo(const std::string& label, size_t* current, const std::string items[], size_t itemsCount, int flags = 0) override;
        void Combo(const std::string& label, size_t* current, const std::vector<std::string>& items, int flags = 0) override;

        void SameLine() override;
        void ToolTip(const std::string& text) override;

        bool Button(const std::string& label) override;
        
    private:
        ImFont* _defaultFont = nullptr;

        bool _drawWorldText = true;
        std::vector<WorldText> _worldTexts;
    };
    
} // namespace Snowglobe::Render
