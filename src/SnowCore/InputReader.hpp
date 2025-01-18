#pragma once

#include <glm/glm.hpp>

namespace Snowglobe::SnowCore
{
    enum KeyStatus : uint8_t
    {
        Up = 0,
        Pressed = 1,
        Released = 2,
        Held = 3
    };

    enum Key : uint16_t
    {
        Space = 32,
        Apostrophe = 39,
        Comma = 44,
        Minus = 45,
        Period = 46,
        Slash = 47,
        Num0 = 48,
        Num1 = 49,
        Num2 = 50,
        Num3 = 51,
        Num4 = 52,
        Num5 = 53,
        Num6 = 54,
        Num7 = 55,
        Num8 = 56,
        Num9 = 57,
        Semicolon = 59,
        Equal = 61,
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        LeftBracket = 91,
        Backslash = 92,
        RightBracket = 93,
        GraveAccent = 96,
        World1 = 161,
        World2 = 162,
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        //Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,

        KP0 = 320,
        KP1 = 321,
        KP2 = 322,
        KP3 = 323,
        KP4 = 324,
        KP5 = 325,
        KP6 = 326,
        KP7 = 327,
        KP8 = 328,
        KP9 = 329,

        KPDecimal = 330,
        KPDivide = 331,
        KPMultiply = 332,
        KPSubtract = 333,
        KPAdd = 334,
        KPEnter = 335,
        KPEqual = 336,

        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,
        Menu = 348
    };

    enum CursorButton : uint16_t
    {
        CursorButton1 = 0,
        CursorButton2 = 1,
        CursorButton3 = 2,
        CursorButton4 = 3,
        CursorButton5 = 4,
        CursorButton6 = 5,
        CursorButton7 = 6,
        CursorButton8 = 7,
        CursorButtonLast = CursorButton8,
        CursorButtonLeft = CursorButton1,
        CursorButtonRight = CursorButton2,
        CursorButtonMiddle = CursorButton3
    };

    enum CursorMode : uint8_t
    {
        CursorModeNormal = 0,
        CursorModeHidden = 1,
        CursorModeDisabled = 2
    };

    class InputReader
    {
    public:

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