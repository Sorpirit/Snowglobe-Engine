#pragma once

#include <glm/glm.hpp>

namespace Snowglobe::RenderOpenGL
{
    class RenderEntity
    {
    public:

        void SetPosition(const glm::vec3& position) 
        { 
            _position = position;
            _dirty = true;
        }
        void SetRotation(const glm::vec3& rotation) 
        { 
            _rotation = rotation; 
            _dirty = true;
        }
        void SetScale(const glm::vec3& scale)
        {
            _scale = scale;
            _dirty = true;
        }

        [[nodiscard]] glm::vec3 Position() const { return _position; }
        [[nodiscard]] glm::vec3 Rotation() const { return _rotation; }
        [[nodiscard]] glm::vec3 Scale() const { return _scale; }

        void Bind(uint32_t pipelineId);
        
    private:
        bool _dirty = true;
        glm::vec3 _position = glm::vec3(0.0f);
        glm::vec3 _rotation = glm::vec3(0.0f);
        glm::vec3 _scale = glm::vec3(1.0f);
        
        glm::mat4 _modelMatrix = glm::mat4(1.0f);
        glm::mat3 _normalMatrix = glm::mat3(1.0f);

        glm::mat4 GetModelMatrix();
    };
    
} // namespace Snowglobe::RenderOpenGL
