#include "SpectatorCameraSystem.hpp"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EngineTime.hpp"
#include "TransformComponent.hpp"

namespace Snowglobe::Engine
{
void SpectatorCameraSystem::Init(std::shared_ptr<Core::ECS::EntityManagerBase> entityManager)
{
    ISystem::Init(entityManager);

    auto camera = _entityManager->CreateEntity(Snowglobe::Tags::MainCamera());
    auto transform = camera->AddOrGetComponent<Core::TransformComponent>();
    transform->Position = glm::vec3(0.0f, 0.0f, 10.0f);
}
void SpectatorCameraSystem::Update()
{
    auto ent = _entityManager->GetEntitiesWithTag(Snowglobe::Tags::MainCamera());
    if (ent.size() < 1)
        return;

    auto cameraEntity = ent[0];
    auto transform = cameraEntity->AddOrGetComponent<Core::TransformComponent>();

    if (_input->IsKeyPressed(Core::Key::F4))
    {
        _isPerspective = !_isPerspective;
        _camera->SetMode(_isPerspective ? Render::CameraMode::Perspective : Render::CameraMode::Orthographic);
    }

    if (_input->IsKeyPressed(Core::Key::F3))
    {
        _mode = _mode == CameraMode::FreeFlyCamera ? CameraMode::Camera2D : CameraMode::FreeFlyCamera;
    }

    if (_mode == CameraMode::FreeFlyCamera)
    {
        if (_input->IsCursorButtonPressed(Core::CursorButton::CursorButtonRight))
        {
            _readRotationInput = true;
            _input->SetCursorMode(Core::CursorMode::CursorModeDisabled);
        }

        if (_input->IsCursorButtonReleased(Core::CursorButton::CursorButtonRight))
        {
            _readRotationInput = false;
            _input->SetCursorMode(Core::CursorMode::CursorModeNormal);
        }
    }

    auto positionChange = ReadPositionInput();
    auto rotationChange = ReadRotationInput();

    float deltaTime = Core::EngineTime::GetDeltaEngineFrameTime();
    float speed = _flySpeed * deltaTime;

    switch (_mode)
    {
    case CameraMode::FreeFlyCamera: {
        // Position
        auto camRotation = transform->Rotation;
        glm::mat4 viewMatrix = glm::mat4(1.0f);
        viewMatrix = glm::rotate(viewMatrix, glm::radians(camRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        viewMatrix = glm::rotate(viewMatrix, glm::radians(camRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        viewMatrix = glm::rotate(viewMatrix, glm::radians(camRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        viewMatrix = glm::inverse(viewMatrix);
        transform->Position += glm::vec3(viewMatrix * glm::vec4(positionChange, 1.0f)) * speed;

        // Rotation
        camRotation.x += rotationChange.x * _mouseSensitivity;
        camRotation.y += rotationChange.y * _mouseSensitivity;
        camRotation.x = std::min(camRotation.x, 89.0f);
        camRotation.x = std::max(camRotation.x, -89.0f);
        transform->Rotation = camRotation;
        break;
    }
    case CameraMode::Camera2D: {
        // Position
        positionChange.y = -positionChange.z;
        positionChange.z = 0;
        transform->Position += positionChange * speed;
        break;
    }
    }

    _camera->SetPosition(transform->Position);
    _camera->SetRotation(transform->Rotation);
}

glm::vec3 SpectatorCameraSystem::ReadPositionInput() const
{
    glm::vec3 move = glm::vec3(0.0f);
    if (_input->IsKeyHeld(Core::Key::W))
    {
        move += glm::vec3(0.0f, 0.0f, -1.0f);
    }
    if (_input->IsKeyHeld(Core::Key::S))
    {
        move += glm::vec3(0.0f, 0.0f, 1.0f);
    }
    if (_input->IsKeyHeld(Core::Key::A))
    {
        move += glm::vec3(-1.0f, 0.0f, 0.0f);
    }
    if (_input->IsKeyHeld(Core::Key::D))
    {
        move += glm::vec3(1.0f, 0.0f, 0.0f);
    }
    if (_input->IsKeyHeld(Core::Key::Q))
    {
        move += glm::vec3(0.0f, -1.0f, 0.0f);
    }
    if (_input->IsKeyHeld(Core::Key::E))
    {
        move += glm::vec3(0.0f, 1.0f, 0.0f);
    }

    return move;
}

glm::vec2 SpectatorCameraSystem::ReadRotationInput() const
{
    if (!_readRotationInput)
        return glm::vec2(0.0f, 0.0f);

    float pitch = _input->GetCursorScreenDelta().y;
    float yaw = _input->GetCursorScreenDelta().x;
    return glm::vec2{pitch, yaw};
}
} // namespace Snowglobe::Engine
