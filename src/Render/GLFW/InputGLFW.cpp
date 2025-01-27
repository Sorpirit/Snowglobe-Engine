#include "InputGLFW.hpp"

#include "WindowGLFW.hpp"

namespace Snowglobe::Render::GLFW
{

    void InputGLFW::KeyCallbackStatic(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        WindowGLFW* instance = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));

        if(instance == nullptr)
            return;

        instance->GetInputGLFW().KeyCallback(window, key, scancode, action, mods);
    }

    void InputGLFW::CursorPositionCallbackStatic(GLFWwindow *window, double xpos, double ypos)
    {
        WindowGLFW* instance = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));

        if(instance == nullptr)
            return;

        instance->GetInputGLFW().CursorPositionCallback(window, xpos, ypos);
    }

    void InputGLFW::CursorButtonCallbackStatic(GLFWwindow *window, int button, int action, int mods)
    {
        WindowGLFW* instance = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));

        if(instance == nullptr)
            return;

        instance->GetInputGLFW().CursorButtonCallback(window, button, action, mods);
    }

    void InputGLFW::CursorScrollCallbackStatic(GLFWwindow *window, double xoffset, double yoffset)
    {
        WindowGLFW* instance = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));

        if(instance == nullptr)
            return;

        instance->GetInputGLFW().CursorScrollCallback(window, xoffset, yoffset);
    }

    int InputGLFW::GetGLFWCursorMode(Core::CursorMode mode)
    {
        switch (mode)
        {
        case Core::CursorMode::CursorModeNormal:
            return GLFW_CURSOR_NORMAL;
        case Core::CursorMode::CursorModeHidden:
            return GLFW_CURSOR_HIDDEN;
        case Core::CursorMode::CursorModeDisabled:
            return GLFW_CURSOR_DISABLED;
        }

        return GLFW_CURSOR_NORMAL;
    }

    void InputGLFW::Init(GLFWwindow *window)
    {
        _window = window;
        glfwSetKeyCallback(window, KeyCallbackStatic);
        glfwSetCursorPosCallback(window, CursorPositionCallbackStatic);
        glfwSetMouseButtonCallback(window, CursorButtonCallbackStatic);
        glfwSetScrollCallback(window, CursorScrollCallbackStatic);
    }

    void InputGLFW::Update()
    {
        for (auto& _key : _keys)
        {
            if (_key == Core::KeyStatus::Pressed)
            {
                _key = Core::KeyStatus::Held;
            }
            else if (_key == Core::KeyStatus::Released)
            {
                _key = Core::KeyStatus::Up;
            }
        }

        for (auto& _mouseButton : _mouseButtons)
        {
            if (_mouseButton == Core::KeyStatus::Pressed)
            {
                _mouseButton = Core::KeyStatus::Held;
            }
            else if (_mouseButton == Core::KeyStatus::Released)
            {
                _mouseButton = Core::KeyStatus::Up;
            }
        }

        _cursorPositionDelta = glm::vec2(0.0f);
        _scrollDelta = 0.0f;
    }

    void InputGLFW::SetCursorScreenPosition(glm::vec2 pos)
    {
        glfwSetCursorPos(_window, pos.x, pos.y);
    }

    void InputGLFW::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        switch (action)
        {
        case GLFW_PRESS:
            _keys[key] = Core::KeyStatus::Pressed;
            break;
        case GLFW_RELEASE:
            _keys[key] = Core::KeyStatus::Released;
            break;
        default:
            break;
        }
    }

    void InputGLFW::CursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
    {
        _cursorPositionDelta = glm::vec2(xpos, ypos) - _cursorPosition;
        _cursorPosition = glm::vec2(xpos, ypos);
    }

    void InputGLFW::CursorButtonCallback(GLFWwindow *window, int button, int action, int mods)
    {
        switch (action)
        {
        case GLFW_PRESS:
            _mouseButtons[button] = Core::KeyStatus::Pressed;
            break;
        case GLFW_RELEASE:
            _mouseButtons[button] = Core::KeyStatus::Released;
            break;
        default:
            break;
        }
    }

    void InputGLFW::CursorScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
    {
        _scroll = static_cast<float>(yoffset);
    }

    void InputGLFW::SetCursorMode(Core::CursorMode mode)
    {
        glfwSetInputMode(_window, GLFW_CURSOR, GetGLFWCursorMode(mode));
    }

} // namespace Snowglobe::Render::GLFW
