#pragma once

#include <functional>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Window.hpp"
#include "InputGLFW.hpp"

namespace Snowglobe::Render::GLFW
{
    class WindowGLFW : public Window
    {
    public:
        WindowGLFW(const WindowParams &params);
        ~WindowGLFW() override;

        void SetResizeCallback(const std::function<void(int, int)>& callback) override;
        void SetCloseCallback(const std::function<void()>& callback) override;
        void Present() override;
        void Focus() override;
        void PollEvents() override;
        void Close() override;
        bool IsOpen() override;
        Core::InputReader& GetInput() override { return _input; }

        GLFWwindow* GetWindow() const { return _window; }
        InputGLFW& GetInputGLFW() { return _input; }
    private:
        WindowParams _params;
        GLFWwindow* _window;
        InputGLFW _input;
        std::function<void(int, int)> _resizeCallback;
        std::function<void()> _closeCallback;

        void WindowResizeCallback(GLFWwindow* windowImpl, int width, int height) const;

        static void WindowResizeCallbackStatic(GLFWwindow* window, int width, int height);
    };
    
} // namespace Snowglobe::Render::GLFW
