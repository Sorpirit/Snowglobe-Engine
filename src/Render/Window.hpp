#pragma once

#include <functional>
#include <string>

#include "InputReader.hpp"

namespace Snowglobe::Render
{
/// @brief Parameters for creating a window.
struct WindowParams
{
    std::string Title;
    uint32_t Width;
    uint32_t Height;
    bool Resizable;
    bool Fullscreen;
    bool Vsync;
};

/// @brief Abstract window class
class Window
{
public:
    virtual ~Window() = default;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    virtual void SetResizeCallback(const std::function<void(int, int)>& callback) = 0;
    virtual void SetCloseCallback(const std::function<void()>& callback) = 0;
    virtual void Present() = 0;
    virtual void Focus() = 0;
    virtual void PollEvents() = 0;
    virtual void Close() { _isOpen = false; }
    virtual bool IsOpen() { return _isOpen; }

    virtual Core::InputReader& GetInput() = 0;
protected:
    bool _isOpen = true;
};
}