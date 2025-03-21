#pragma once

#include <cstdint>
#include "string"
#include "glm/glm.hpp"

#include "Texture.hpp"
#include "Texture2DPtr.hpp"
#include "TextureManager.hpp"

#include "glad/gl.h"

namespace Snowglobe::RenderOpenGL
{

    class UniformLocationSetter
    {
    public:
        UniformLocationSetter(uint32_t pipelineId) : _pipelineId(pipelineId) {}

        void Set(std::string_view variable, bool value)
        {
            glUniform1i(ReadLocation(variable), value);
        }

        void Set(std::string_view variable, int value)
        {
            glUniform1i(ReadLocation(variable), value);
        }

        void Set(std::string_view variable, uint32_t value)
        {
            glUniform1ui(ReadLocation(variable), value);
        }

        void Set(std::string_view variable, float value)
        {
            glUniform1f(ReadLocation(variable), value);
        }

        void Set(std::string_view variable, const glm::vec2& value)
        {
            glUniform2fv(ReadLocation(variable), 1, &value[0]);
        }

        void Set(std::string_view variable, const glm::vec3& value)
        {
            glUniform3fv(ReadLocation(variable), 1, &value[0]);
        }

        void Set(std::string_view variable, const glm::vec4& value)
        {
            glUniform4fv(ReadLocation(variable), 1, &value[0]);
        }

        void Set(std::string_view variable, const glm::mat3& value)
        {
            glUniformMatrix3fv(ReadLocation(variable), 1, GL_FALSE, &value[0][0]);
        }

        void Set(std::string_view variable, const glm::mat4& value)
        {
            glUniformMatrix4fv(ReadLocation(variable), 1, GL_FALSE, &value[0][0]);
        }

        void Set(std::string_view variable, const Render::Texture2DPtr& texture)
        {
            glActiveTexture(GL_TEXTURE0 + _textureCounter);
            TextureManager::GetInstance().GetTexture(texture).Bind();
            // glActiveTexture()
            glUniform1i(ReadLocation(variable), _textureCounter);
            _textureCounter++;
        }

        void SetArray(std::string_view variable, const glm::vec2* value, size_t count)
        {
            glUniform2fv(ReadLocation(variable), count, &value[0][0]);
        }

        void SetArray(std::string_view variable, const glm::vec3* value, size_t count)
        {
            glUniform3fv(ReadLocation(variable), count, &value[0][0]);
        }

        void SetArray(std::string_view variable, const glm::vec4* value, size_t count)
        {
            glUniform4fv(ReadLocation(variable), count, &value[0][0]);
        }

        void SetArray(std::string_view variable, const glm::mat3* value, size_t count)
        {
            glUniformMatrix3fv(ReadLocation(variable), count, GL_FALSE, &value[0][0][0]);
        }

        void SetArray(std::string_view variable, const glm::mat4* value, size_t count)
        {
            glUniformMatrix4fv(ReadLocation(variable), count, GL_FALSE, &value[0][0][0]);
        }

    private:
        uint32_t _pipelineId;
        uint32_t _textureCounter = 0;

        uint32_t ReadLocation(std::string_view variable)
        {
            uint32_t location = glGetUniformLocation(_pipelineId, variable.data());
            return location;
        }
    };
}