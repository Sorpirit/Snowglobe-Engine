#include "InputGLFW.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

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
    
    void InputGLFW::Init(GLFWwindow *window)
    {
        glfwSetKeyCallback(window, KeyCallbackStatic);
        glfwSetCursorPosCallback(window, CursorPositionCallbackStatic);
        glfwSetMouseButtonCallback(window, CursorButtonCallbackStatic);
        glfwSetScrollCallback(window, CursorScrollCallbackStatic);
    }

    void InputGLFW::Update()
    {
        for (int i = 0; i < GLFW_KEY_LAST + 1; i++)
        {
            if (_keys[i] == SnowCore::KeyStatus::Pressed)
            {
                _keys[i] = SnowCore::KeyStatus::Held;
            }
            else if (_keys[i] == SnowCore::KeyStatus::Released)
            {
                _keys[i] = SnowCore::KeyStatus::Up;
            }
        }

        for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST + 1; i++)
        {
            if (_mouseButtons[i] == SnowCore::KeyStatus::Pressed)
            {
                _mouseButtons[i] = SnowCore::KeyStatus::Held;
            }
            else if (_mouseButtons[i] == SnowCore::KeyStatus::Released)
            {
                _mouseButtons[i] = SnowCore::KeyStatus::Up;
            }
        }

        _cursorPositionDelta = glm::vec2(0.0f);
        _scrollDelta = 0.0f;
    }

    void InputGLFW::SetCursorScreenPosition(glm::vec2 pos)
    {
        //glfwSetCursorPos(_window, pos.x, pos.y);
    }

    void InputGLFW::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        switch (action)
        {
        case GLFW_PRESS:
            _keys[key] = SnowCore::KeyStatus::Pressed;
            break;
        case GLFW_RELEASE:
            _keys[key] = SnowCore::KeyStatus::Released;
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
            _mouseButtons[button] = SnowCore::KeyStatus::Pressed;
            break;
        case GLFW_RELEASE:
            _mouseButtons[button] = SnowCore::KeyStatus::Released;
            break;
        }
    }

    void InputGLFW::CursorScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
    {
        _scroll = yoffset;
    }

    } // namespace Snowglobe::Render::GLFW
