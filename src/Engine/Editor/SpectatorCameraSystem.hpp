#pragma once

#include "Camera.hpp"
#include "ECS/Component.hpp"
#include "ECS/ISystem.hpp"
#include "InputReader.hpp"

namespace Snowglobe::Engine
{

REGISTER_TAG(MainCamera);

enum class CameraMode : uint8_t
{
    FreeFlyCamera = 0,
    Camera2D = 1
};

class SpectatorCameraSystem : public Core::ECS::ISystem
{
  public:
    SpectatorCameraSystem(Render::Camera* camera, Core::InputReader* input) : _camera(camera), _input(input) {}

    void Init(std::shared_ptr<Core::ECS::EntityManagerBase> entityManager) override;
    void Update() override;
  private:
    Render::Camera* _camera;
    Core::InputReader* _input;

    CameraMode _mode = CameraMode::FreeFlyCamera;

    float _flySpeed = 5.0f;
    float _mouseSensitivity = 0.08f;
    bool _readRotationInput = false;
    bool _isPerspective = false;

    glm::vec3 ReadPositionInput() const;
    glm::vec2 ReadRotationInput() const;
};
} // namespace Snowglobe::Engine
