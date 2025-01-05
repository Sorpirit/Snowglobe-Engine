#pragma once

#include <string>
#include <functional>

namespace Snowglobe::Render
{
    /// @brief Parameters for creating a window.
    struct WindowParams
    {
        std::string title;
        unsigned int width;
        unsigned int height;
        int x;
        int y;
        bool resizable;
        bool fullscreen;
        bool vsync;
        bool cursorVisible;
        bool cursorLocked;
        double cursorPosX;
        double cursorPosY;
        std::string iconPath;
    };

    /// @brief 
    class Window
    {
    public:
        Window() = default;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        virtual void SetResizeCallback(const std::function<void(int, int)>& callback) = 0;
        virtual void SetCloseCallback(const std::function<void()>& callback) = 0;
        virtual void Present() = 0;
        virtual void Focus() = 0;
        virtual void PollEvents() = 0;
        virtual void Close() = 0;
        virtual bool IsOpen() { return _isOpen; }
    protected:
        bool _isOpen = true;
    };
}