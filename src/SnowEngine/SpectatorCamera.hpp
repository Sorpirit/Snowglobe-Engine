#pragma once

#include <glm/glm.hpp>

#include "Camera.hpp"
#include "InputReader.hpp"

namespace Snowglobe::SnowEngine
{
    class SpectatorCamera
    {
    public:
        SpectatorCamera(Render::Camera& camera, SnowCore::InputReader& input) : _camera(camera), _input(input) {}

        void Update();
    private:
        Render::Camera& _camera;
        SnowCore::InputReader& _input;

        bool _isMoving = false;
        float _flySpeed = 5.0f;
        float _mouseSensitivity = 0.08f;
    };
}
