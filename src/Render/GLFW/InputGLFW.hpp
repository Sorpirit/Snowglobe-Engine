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

        SnowCore::KeyStatus GetKeyStatus(uint32_t key) const override { return _keys[key]; }
        bool IsKeyUp(uint32_t key) const override { return _keys[key] == SnowCore::KeyStatus::Up; }
        bool IsKeyPressed(uint32_t key) const override { return _keys[key] == SnowCore::KeyStatus::Pressed; }
        bool IsKeyReleased(uint32_t key) const override { return _keys[key] == SnowCore::KeyStatus::Released; }
        bool IsKeyHeld(uint32_t key) const override { return _keys[key] == SnowCore::KeyStatus::Held; }

        SnowCore::KeyStatus GetCursorButtonStatus(uint32_t button) const override { return _mouseButtons[button]; }
        bool IsCursorButtonUp(uint32_t button) const override { return _mouseButtons[button] == SnowCore::KeyStatus::Up; }
        bool IsCursorButtonPressed(uint32_t button) const override { return _mouseButtons[button] == SnowCore::KeyStatus::Pressed; }
        bool IsCursorButtonReleased(uint32_t button) const override { return _mouseButtons[button] == SnowCore::KeyStatus::Released; }
        bool IsCursorButtonHeld(uint32_t button) const override { return _mouseButtons[button] == SnowCore::KeyStatus::Held; }
        glm::vec2 GetCursorScreenPosition() const override { return _cursorPosition; }
        glm::vec2 GetCursorScreenDelta() const override { return _cursorPositionDelta; }
        float GetScroll() const override { return _scroll; }
        float GetScrollDelta() const override { return _scrollDelta; }

        void SetCursorScreenPosition(glm::vec2 pos) override;

        void SetCursorMode(SnowCore::CursorMode mode) override;

    private:
        GLFWwindow* _window;
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

        static int GetGLFWCursorMode(SnowCore::CursorMode mode);
    };
} // namespace Snowglobe::Render::GLFW