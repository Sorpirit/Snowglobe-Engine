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
    
    class ImguiSystem : public Render::UISystem, public SnowCore::ISnowFrameSystem
    {
    public:
        ImguiSystem(Render::Window* window);
        ~ImguiSystem() override;

        void EarlyUpdate() override;
        void Update() override;

        Render::UIPanel OpenUIPanel(const std::string& title) override;
        void Close(Render::UIPanel* panel) override;
        
        void AddWorldText(const glm::vec3& position, const std::string& text, const glm::vec3& color = {1.0f, 1.0f, 0.0f}, int alignment = 0) override
        {
            if (!_drawWorldText)
                return;
            _worldTexts.emplace_back(text, position, color, alignment);
        }
        
        void SetDefaultFont(SnowCore::SnowFileHandle fontFileHandle) override;

        void Text(const std::string& text) override;
        void Text(const char* fmt, ...) override;
        
        bool Checkbox(const std::string& label, bool* value) override;
        
        void Slider(const std::string& label, float* value, float min, float max, const std::string& format = "%.3f", int flags = Render::Slider::None) override;
        void Slider(const std::string& label, glm::vec2* value, float min, float max, const std::string& format = "%.3f", int flags = Render::Slider::None) override;
        void Slider(const std::string& label, glm::vec3* value, float min, float max, const std::string& format = "%.3f", int flags = Render::Slider::None) override;
        void Slider(const std::string& label, glm::vec4* value, float min, float max, const std::string& format = "%.3f", int flags = Render::Slider::None) override;
        void Slider(const std::string& label, int* value, float min, float max, const std::string& format = "%d", int flags = Render::Slider::None) override;
        void Slider(const std::string& label, glm::ivec2* value, float min, float max, const std::string& format = "%d", int flags = Render::Slider::None) override;
        void Slider(const std::string& label, glm::ivec3* value, float min, float max, const std::string& format = "%d", int flags = Render::Slider::None) override;
        void Slider(const std::string& label, glm::ivec4* value, float min, float max, const std::string& format = "%d", int flags = Render::Slider::None) override;
        void SliderAngle(const std::string& label, float* value, float min, float max, const std::string& format = "%.3f", int flags = Render::Slider::None) override;

        void Color(const std::string& label, glm::vec3* value, int flags = 0) override;
        void Color(const std::string& label, glm::vec4* value, int flags = 0) override;

        void Combo(const std::string& label, int* current, const std::string items[], int itemsCount, int flags = 0) override;
        void Combo(const std::string& label, int* current, const std::vector<std::string>& items, int flags = 0) override;

        void InputText(const std::string& label, char* textBuff, size_t size, int flags = 0) override;
        
    private:
        ImFont* _defaultFont = nullptr;

        bool _drawWorldText = true;
        std::vector<WorldText> _worldTexts;
    };
    
} // namespace Snowglobe::Render
