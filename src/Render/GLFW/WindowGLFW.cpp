#include "WindowGLFW.hpp"

namespace Snowglobe::Render::GLFW
{
    void WindowGLFW::WindowResizeCallbackStatic(GLFWwindow* window, int width, int height) {
        // Retrieve the Window instance using glfwGetWindowUserPointer
        if (WindowGLFW* instance = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window))) {
            // Call the non-static member function
            instance->WindowResizeCallback(window, width, height);
        }
    }

    WindowGLFW::WindowGLFW(const WindowParams &params) :
        _params(params),
        _window(nullptr),
        _resizeCallback(nullptr),
        _closeCallback(nullptr)
    {
        glfwWindowHint(GLFW_RESIZABLE, params.Resizable);
        _window = glfwCreateWindow(static_cast<int>(params.Width), static_cast<int>(params.Height), params.Title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(_window, this);
        glfwMakeContextCurrent(_window);
    
        glfwSetFramebufferSizeCallback(_window, WindowResizeCallbackStatic);

        _input.Init(_window);
    }
    
    WindowGLFW::~WindowGLFW()
    {
        glfwDestroyWindow(_window);
    }

    void WindowGLFW::SetResizeCallback(const std::function<void(int, int)>& callback)
    {
        _resizeCallback = callback;
    }

    void WindowGLFW::SetCloseCallback(const std::function<void()>& callback)
    {
        _closeCallback = callback;
    }

    void WindowGLFW::Present()
    {
        glfwSwapBuffers(_window);
    }

    void WindowGLFW::Focus()
    {
        glfwFocusWindow(_window);
    }

    void WindowGLFW::PollEvents()
    {
        _input.Update();
        glfwPollEvents();
    }

    void WindowGLFW::Close()
    {   
        if(!_isOpen)
            return;

        Window::Close();
        
        glfwSetWindowShouldClose(_window, true);
        if(_closeCallback)
            _closeCallback();
    }

    bool WindowGLFW::IsOpen()
    {
        if(!_isOpen)
            return false;

        _isOpen = !glfwWindowShouldClose(_window);
        if(!_isOpen && _closeCallback)
            _closeCallback();

        return _isOpen;
    }

    void WindowGLFW::WindowResizeCallback(GLFWwindow *windowImpl, int width, int height) const
    {
        if(_resizeCallback)
            _resizeCallback(width, height);
    }

}