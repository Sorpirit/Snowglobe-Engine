#pragma once

#include <InputReader.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Snowglobe::Render::GLFW
{
    class InputGLFW : public Core::InputReader
    {
    public:
        void Init(GLFWwindow* window);
        void Update();

        Core::KeyStatus GetKeyStatus(uint32_t key) const override { return _keys[key]; }
        bool IsKeyUp(uint32_t key) const override { return _keys[key] == Core::KeyStatus::Up; }
        bool IsKeyPressed(uint32_t key) const override { return _keys[key] == Core::KeyStatus::Pressed; }
        bool IsKeyReleased(uint32_t key) const override { return _keys[key] == Core::KeyStatus::Released; }
        bool IsKeyHeld(uint32_t key) const override { return _keys[key] == Core::KeyStatus::Held; }

        Core::KeyStatus GetCursorButtonStatus(uint32_t button) const override { return _mouseButtons[button]; }
        bool IsCursorButtonUp(uint32_t button) const override { return _mouseButtons[button] == Core::KeyStatus::Up; }
        bool IsCursorButtonPressed(uint32_t button) const override { return _mouseButtons[button] == Core::KeyStatus::Pressed; }
        bool IsCursorButtonReleased(uint32_t button) const override { return _mouseButtons[button] == Core::KeyStatus::Released; }
        bool IsCursorButtonHeld(uint32_t button) const override { return _mouseButtons[button] == Core::KeyStatus::Held; }
        glm::vec2 GetCursorScreenPosition() const override { return _cursorPosition; }
        glm::vec2 GetCursorScreenDelta() const override { return _cursorPositionDelta; }
        float GetScroll() const override { return _scroll; }
        float GetScrollDelta() const override { return _scrollDelta; }

        void SetCursorScreenPosition(glm::vec2 pos) override;

        void SetCursorMode(Core::CursorMode mode) override;

    private:
        GLFWwindow* _window = nullptr;
        Core::KeyStatus _keys[GLFW_KEY_LAST + 1] = {};
        Core::KeyStatus _mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = {};
        glm::vec2 _cursorPosition = glm::vec2(0, 0);
        glm::vec2 _cursorPositionDelta = glm::vec2(0, 0);
        float _scroll = 0;
        float _scrollDelta = 0;

        void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
        void CursorButtonCallback(GLFWwindow* window, int button, int action, int mods);
        void CursorScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        static void KeyCallbackStatic(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void CursorPositionCallbackStatic(GLFWwindow* window, double xpos, double ypos);
        static void CursorButtonCallbackStatic(GLFWwindow* window, int button, int action, int mods);
        static void CursorScrollCallbackStatic(GLFWwindow* window, double xoffset, double yoffset);

        static int GetGLFWCursorMode(Core::CursorMode mode);
    };
} // namespace Snowglobe::Render::GLFW