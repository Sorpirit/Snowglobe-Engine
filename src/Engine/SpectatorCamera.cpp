#include "SpectatorCamera.hpp"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EngineTime.hpp"

namespace Snowglobe::Engine
{
    void SpectatorCamera::Update()
    {
        bool hasMoved = false;
        glm::vec3 move = glm::vec3(0.0f);
        if (_input.IsKeyHeld(Core::Key::W))
        {
            move += glm::vec3(0.0f, 0.0f, -1.0f);
            hasMoved = true;
        }
        if (_input.IsKeyHeld(Core::Key::S))
        {
            move += glm::vec3(0.0f, 0.0f, 1.0f);
            hasMoved = true;
        }
        if (_input.IsKeyHeld(Core::Key::A))
        {
            move += glm::vec3(-1.0f, 0.0f, 0.0f);
            hasMoved = true;
        }
        if (_input.IsKeyHeld(Core::Key::D))
        {
            move += glm::vec3(1.0f, 0.0f, 0.0f);
            hasMoved = true;
        }
        if (_input.IsKeyHeld(Core::Key::Q))
        {
            move += glm::vec3(0.0f, -1.0f, 0.0f);
            hasMoved = true;
        }
        if (_input.IsKeyHeld(Core::Key::E))
        {
            move += glm::vec3(0.0f, 1.0f, 0.0f);
            hasMoved = true;
        }
        
        if (hasMoved)
        {
            float deltaTime = Core::EngineTime::GetDeltaEngineFrameTime();
            float speed = _flySpeed * deltaTime;
            
            auto camRotation = _camera.GetRotation();
            glm::mat4 viewMatrix = glm::mat4(1.0f);
            viewMatrix = glm::rotate(viewMatrix, glm::radians(camRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            viewMatrix = glm::rotate(viewMatrix, glm::radians(camRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            viewMatrix = glm::rotate(viewMatrix, glm::radians(camRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            viewMatrix = glm::inverse(viewMatrix);

            _camera.SetPosition(_camera.GetPosition() + glm::vec3(viewMatrix * glm::vec4(move, 1.0f)) * speed);
        }

        if (_input.IsCursorButtonPressed(Core::CursorButton::CursorButtonRight))
        {
            _isMoving = true;
            _input.SetCursorMode(Core::CursorMode::CursorModeDisabled);
        }

        if (_input.IsCursorButtonReleased(Core::CursorButton::CursorButtonRight))
        {
            _isMoving = false;
            _input.SetCursorMode(Core::CursorMode::CursorModeNormal);
        }

        if (_isMoving)
        {
            float pitch = _input.GetCursorScreenDelta().y * _mouseSensitivity;
            float yaw = _input.GetCursorScreenDelta().x * _mouseSensitivity;

            auto camRotation = _camera.GetRotation();
            camRotation.x += pitch;
            camRotation.y += yaw;

            camRotation.x = std::min(camRotation.x, 89.0f);
            camRotation.x = std::max(camRotation.x, -89.0f);

            _camera.SetRotation(camRotation);
        }
    }
}


