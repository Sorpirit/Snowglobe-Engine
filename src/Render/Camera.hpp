#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Snowglobe::Render
{
    enum CameraMode
    {
        Orthographic,
        Perspective
    };

    class Camera
    {
    public:
        Camera() = default;
        Camera(const Camera&) = delete;
        Camera& operator=(const Camera&) = delete;
        Camera(Camera&&) = delete;
        Camera& operator=(Camera&&) = delete;

        Camera(CameraMode mode, float fov, uint32_t width, uint32_t height, float near, float far, float orthographicSize = 1)
            : _mode(mode), _fov(fov), _width(width), _height(height), _aspectRatio(static_cast<float>(_width) / static_cast<float>(_height)), _orthographicSize(orthographicSize), _near(near), _far(far),
            _projectionMatrix(GenerateProjectionMatrix())
        { 
        }

        void SetPosition(const glm::vec3& position) 
        { 
            _position = position; 
            _viewDirty = true;
        }
        void SetRotation(const glm::vec3& rotation) 
        {   
            _rotation = rotation;
            _viewDirty = true;
        }

        const glm::vec3& GetPosition() const { return _position; }
        const glm::vec3& GetRotation() const { return _rotation; }

        const glm::mat4 GetViewMatrix() const
        {
            return _viewMatrix;
        }

        const glm::mat4 GetProjectionMatrix() const
        {
            return _projectionMatrix;
        }

        const glm::mat4 GetViewProjectionMatrix() const
        {
            return _projectionMatrix * GetViewMatrix();
        }

        void SetMode(CameraMode mode)
        {
            _mode = mode;
            _projDirty = true;
        }

        void SetFov(float fov)
        {
            _fov = fov;
            _projDirty = true;
        }

        void SetWidth(uint32_t width)
        {
            _width = width;
            _projDirty = true;
        }

        void SetHeight(uint32_t height)
        {
            _height = height;
            _projDirty = true;
        }

        void SetOrthographicSize(float orthographicSize)
        {
            _orthographicSize = orthographicSize;
            _projDirty = true;
        }

        void SetNear(float near)
        {
            _near = near;
            _projDirty = true;
        }

        void SetFar(float far)
        {
            _far = far;
            _projDirty = true;
        }
        
        void Update()
        {
            RecalculateMatrices();
        }

        glm::vec3 ScreenToWorld(const glm::vec2& screenPosition) const
        {
            glm::vec4 viewport = glm::vec4(0.0f, 0.0f, static_cast<float>(_width), static_cast<float>(_height));
            glm::vec3 screenPosition3D = glm::vec3(screenPosition, 0.0f);

            glm::vec3 worldPosition = glm::unProject(screenPosition3D, _viewMatrix, _projectionMatrix, viewport);

            return worldPosition;
        }

        glm::vec2 WorldToScreen(const glm::vec3& worldPosition) const
        {
            glm::vec4 viewport = glm::vec4(0.0f, 0.0f, static_cast<float>(_width), static_cast<float>(_height));

            glm::vec3 screenPosition = glm::project(worldPosition, _viewMatrix, _projectionMatrix, viewport);

            screenPosition.y = _height - screenPosition.y;
            
            return glm::vec2(screenPosition);
        }

    private:
        bool _viewDirty = true;
        glm::mat4 _viewMatrix = glm::mat4(1.0f);
        glm::vec3 _position = glm::vec3(0.0f);
        glm::vec3 _rotation = glm::vec3(0.0f);

        bool _projDirty = true;
        glm::mat4 _projectionMatrix = glm::mat4(1.0f);
        CameraMode _mode = CameraMode::Perspective;
        float _fov = 45.0f;
        uint32_t _width = 0;
        uint32_t _height = 0;
        float _aspectRatio = 1.0f;
        float _orthographicSize = 1.0f;
        float _near = 0.1f;
        float _far = 100.0f;

        void RecalculateMatrices()
        {
            if(_viewDirty)
            {
                _viewMatrix = GenerateViewMatrix();
                _viewDirty = false;
            }

            if(_projDirty)
            {
                _projectionMatrix = GenerateProjectionMatrix();
                _projDirty = false;
            }
        }

        glm::mat4 GenerateViewMatrix()
        {
            glm::mat4 viewMatrix = glm::mat4(1.0f);
            viewMatrix = glm::rotate(viewMatrix, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            viewMatrix = glm::rotate(viewMatrix, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            viewMatrix = glm::rotate(viewMatrix, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            viewMatrix = glm::translate(viewMatrix, _position);

            return viewMatrix;
        }

        glm::mat4 GenerateProjectionMatrix()
        {
            _aspectRatio = static_cast<float>(_width) / static_cast<float>(_height);

            if(_mode == CameraMode::Orthographic)
            {
                float horizontal = (_orthographicSize * _aspectRatio) / 2;
                float vertical = _orthographicSize / 2;

                return glm::ortho(-horizontal, horizontal, -vertical, vertical, _near, _far);
            }
            else
            {
                return glm::perspective(glm::radians(_fov), _aspectRatio, _near, _far);
            }
        }
    };
} // namespace Snowglobe::Render
