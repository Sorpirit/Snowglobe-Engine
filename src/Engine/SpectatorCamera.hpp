#pragma once

#include "Camera.hpp"
#include "InputReader.hpp"

namespace Snowglobe::Engine
{
    class SpectatorCamera
    {
    public:
        SpectatorCamera(Render::Camera& camera, Core::InputReader& input) : _camera(camera), _input(input) {}

        void Update();
    private:
        Render::Camera& _camera;
        Core::InputReader& _input;

        bool _isMoving = false;
        float _flySpeed = 5.0f;
        float _mouseSensitivity = 0.08f;
    };
}
