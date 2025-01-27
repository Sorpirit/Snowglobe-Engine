#pragma once

#include <glm/glm.hpp>

#include "InputConstants.hpp"

namespace Snowglobe::Core
{
    
class InputReader
{
public:
    virtual ~InputReader() = default;

    virtual KeyStatus GetKeyStatus(uint32_t key) const = 0;
    virtual bool IsKeyUp(uint32_t key) const = 0;
    virtual bool IsKeyPressed(uint32_t key) const = 0;
    virtual bool IsKeyReleased(uint32_t key) const = 0;
    virtual bool IsKeyHeld(uint32_t key) const = 0;


    virtual KeyStatus GetCursorButtonStatus(uint32_t button) const = 0;
    virtual bool IsCursorButtonUp(uint32_t button) const = 0;
    virtual bool IsCursorButtonPressed(uint32_t button) const = 0;
    virtual bool IsCursorButtonReleased(uint32_t button) const = 0;
    virtual bool IsCursorButtonHeld(uint32_t button) const = 0;
    virtual glm::vec2 GetCursorScreenPosition() const = 0; 
    virtual glm::vec2 GetCursorScreenDelta() const = 0;
    virtual float GetScroll() const = 0;
    virtual float GetScrollDelta() const = 0;

    virtual void SetCursorScreenPosition(glm::vec2 pos) = 0;
    virtual void SetCursorMode(CursorMode mode) = 0;
};
    
}