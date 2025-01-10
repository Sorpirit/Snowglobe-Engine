#pragma once

#include <InputReader.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Snowglobe::Render::GLFW
{
    class InputGLFW : public SnowCore::InputReader
    {
    public:
        void Init(GLFWwindow* window);
        void Update();

        virtual SnowCore::KeyStatus GetKeyStatus(uint32_t key) override { return _keys[key]; }
        virtual bool IsKeyUp(uint32_t key) override { return _keys[key] == SnowCore::KeyStatus::Up; }
        virtual bool IsKeyPressed(uint32_t key) override { return _keys[key] == SnowCore::KeyStatus::Pressed; }
        virtual bool IsKeyReleased(uint32_t key) override { return _keys[key] == SnowCore::KeyStatus::Released; }
        virtual bool IsKeyHeld(uint32_t key) override { return _keys[key] == SnowCore::KeyStatus::Held; }

        virtual SnowCore::KeyStatus GetCursorButtonStatus(uint32_t button) override { return _mouseButtons[button]; }
        virtual bool IsCursorButtonUp(uint32_t button) override { return _mouseButtons[button] == SnowCore::KeyStatus::Up; }
        virtual bool IsCursorButtonPressed(uint32_t button) override { return _mouseButtons[button] == SnowCore::KeyStatus::Pressed; }
        virtual bool IsCursorButtonReleased(uint32_t button) override { return _mouseButtons[button] == SnowCore::KeyStatus::Released; }
        virtual bool IsCursorButtonHeld(uint32_t button) override { return _mouseButtons[button] == SnowCore::KeyStatus::Held; }
        virtual glm::vec2 GetCursorScreenPosition() override { return _cursorPosition; }
        virtual glm::vec2 GetCursorScreenDelta() override { return _cursorPositionDelta; }
        virtual float GetScroll() override { return _scroll; }
        virtual float GetScrollDelta() override { return _scrollDelta; }

        virtual void SetCursorScreenPosition(glm::vec2 pos) override;

    private:
        SnowCore::KeyStatus _keys[GLFW_KEY_LAST + 1];
        SnowCore::KeyStatus _mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];
        glm::vec2 _cursorPosition;
        glm::vec2 _cursorPositionDelta;
        float _scroll;
        float _scrollDelta;

        void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
        void CursorButtonCallback(GLFWwindow* window, int button, int action, int mods);
        void CursorScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        static void KeyCallbackStatic(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void CursorPositionCallbackStatic(GLFWwindow* window, double xpos, double ypos);
        static void CursorButtonCallbackStatic(GLFWwindow* window, int button, int action, int mods);
        static void CursorScrollCallbackStatic(GLFWwindow* window, double xoffset, double yoffset);
    };
    
} // namespace Snowglobe::Render::GLFW