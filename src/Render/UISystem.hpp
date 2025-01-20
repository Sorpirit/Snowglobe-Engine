#pragma once

#include <memory>
#include <vector>
#include <string>

#include <glm/glm.hpp>

#include <ECS/ISystem.hpp>
#include <SnowFileSystem.hpp>

namespace Snowglobe::Render
{
    class UISystem;

    class UIPanel
    {
    public:
        UIPanel(UISystem* system) : _system(system) {}
        ~UIPanel();

        bool Close() 
        {
            if(!_isOpen)
                return false;

            _isOpen = false; 
            return true;
        }
    private:
        UISystem* _system;
        bool _isOpen = true;
    };

    enum Alignment
    {
        // vertical:horizontal 00:00
        // center:center 01:01
        // right:bottom 10:10
        
        Default          = 0,
        HorizontalLeft   = 0,
        HorizontalCenter = 1 << 0,
        HorizontalRight  = 1 << 1,
        VerticalTop      = 0,
        VerticalCenter   = 1 << 2,
        VerticalBottom   = 1 << 3
    };

    enum Slider
    {
        None            = 0,
        Logarithmic     = 1 << 5,
        NoRoundToFormat = 1 << 6,
        NoInput         = 1 << 7,
        WrapAround      = 1 << 8,
        ClampOnInput    = 1 << 9,
        ClampZeroRange  = 1 << 10,
        AlwaysClamp     = ClampOnInput | ClampZeroRange
    };

    class UISystem : public SnowCore::ECS::ISystem
    {
    public:
        UISystem(SnowCore::ECS::EntityManagerBase& entityManager) : ISystem(entityManager)  {}

        virtual UIPanel OpenUIPanel(const std::string& title) = 0;
        virtual void Close(UIPanel* panel) = 0;

        virtual void Text(const std::string& text) = 0;
        virtual void Text(const char* fmt, ...) = 0;
        
        virtual bool Checkbox(const std::string& label, bool* value) = 0;

        virtual bool Input(const std::string& label, char* textBuff, size_t size, int flags = 0) = 0;
        virtual bool Input(const std::string& label, float* value, float step, float stepFast, const std::string& format = "%.3f", int flags = 0) = 0;
        virtual bool Input(const std::string& label, glm::vec2* value, const std::string& format = "%.3f", int flags = 0) = 0;
        virtual bool Input(const std::string& label, glm::vec3* value, const std::string& format = "%.3f", int flags = 0) = 0;
        virtual bool Input(const std::string& label, glm::vec4* value, const std::string& format = "%.3f", int flags = 0) = 0;
        virtual bool Input(const std::string& label, int* value, int step, int stepFast, int flags = 0) = 0;
        virtual bool Input(const std::string& label, glm::ivec2* value, int flags = 0) = 0;
        virtual bool Input(const std::string& label, glm::ivec3* value, int flags = 0) = 0;
        virtual bool Input(const std::string& label, glm::ivec4* value, int flags = 0) = 0;

        virtual bool Drag(const std::string& label, float* value, float speed = 0, float min = 0, float max = 0, const std::string& format = "%.3f", int flags = 0) = 0;
        virtual bool Drag(const std::string& label, glm::vec2* value, float speed = 0, float min = 0, float max = 0, const std::string& format = "%.3f", int flags = 0) = 0;
        virtual bool Drag(const std::string& label, glm::vec3* value, float speed = 0, float min = 0, float max = 0, const std::string& format = "%.3f", int flags = 0) = 0;
        virtual bool Drag(const std::string& label, glm::vec4* value, float speed = 0, float min = 0, float max = 0, const std::string& format = "%.3f", int flags = 0) = 0;
        virtual bool Drag(const std::string& label, int* value, float speed = 0, int min = 0, int max = 0, const std::string& format = "%d", int flags = 0) = 0;
        virtual bool Drag(const std::string& label, glm::ivec2* value, float speed = 0, int min = 0, int max = 0, const std::string& format = "%d", int flags = 0) = 0;
        virtual bool Drag(const std::string& label, glm::ivec3* value, float speed = 0, int min = 0, int max = 0, const std::string& format = "%d", int flags = 0) = 0;
        virtual bool Drag(const std::string& label, glm::ivec4* value, float speed = 0, int min = 0, int max = 0, const std::string& format = "%d", int flags = 0) = 0;
        
        virtual void Slider(const std::string& label, float* value, float min, float max, const std::string& format = "%.3f", int flags = Slider::None) = 0;
        virtual void Slider(const std::string& label, glm::vec2* value, float min, float max, const std::string& format = "%.3f", int flags = Slider::None) = 0;
        virtual void Slider(const std::string& label, glm::vec3* value, float min, float max, const std::string& format = "%.3f", int flags = Slider::None) = 0;
        virtual void Slider(const std::string& label, glm::vec4* value, float min, float max, const std::string& format = "%.3f", int flags = Slider::None) = 0;
        virtual void Slider(const std::string& label, int* value, float min, float max, const std::string& format = "%d", int flags = Slider::None) = 0;
        virtual void Slider(const std::string& label, glm::ivec2* value, float min, float max, const std::string& format = "%d", int flags = Slider::None) = 0;
        virtual void Slider(const std::string& label, glm::ivec3* value, float min, float max, const std::string& format = "%d", int flags = Slider::None) = 0;
        virtual void Slider(const std::string& label, glm::ivec4* value, float min, float max, const std::string& format = "%d", int flags = Slider::None) = 0;
        virtual void SliderAngle(const std::string& label, float* value, float min, float max, const std::string& format = "%.3f", int flags = Slider::None) = 0;

        virtual void Color(const std::string& label, glm::vec3* value, int flags = 0) = 0;
        virtual void Color(const std::string& label, glm::vec4* value, int flags = 0) = 0;

        virtual void Combo(const std::string& label, int* current, const std::string items[], int itemsCount, int flags = 0) = 0;
        virtual void Combo(const std::string& label, int* current, const std::vector<std::string>& items, int flags = 0) = 0;

        virtual void SameLine() = 0;
        virtual void ToolTip(const std::string& text) = 0;

        virtual bool Button(const std::string& label) = 0;
        
        virtual void AddWorldText(const glm::vec3& position, const std::string& text, const glm::vec3& color = {1.0f, 1.0f, 0.0f}, int alignment = 0) = 0;
        virtual void SetDefaultFont(SnowCore::SnowFileHandle fontFileHandle) = 0;
    };
    
} // namespace Snowglobe::Render
