#pragma once

#include <array>
#include <cstdint>
#include "string"
#include "glm/glm.hpp"

#include "Texture.hpp"
#include "Texture2DPtr.hpp"
#include "TextureManager.hpp"

#include "glad/gl.h"

namespace Snowglobe::RenderOpenGL
{
    template <size_t N>
    class UniformLocationSetter;
    

    template <size_t N>
    class CachedUniformLocations
    {
    public:
        CachedUniformLocations() = default;
    
        UniformLocationSetter<N> GetSetter(uint32_t pipelineId)
        {
            bool recache = _lastPipelineId != pipelineId;
            _lastPipelineId = pipelineId;
            return UniformLocationSetter<N>(pipelineId, recache, *this);
        }

        uint32_t GetLocation(const std::string& variable, uint32_t pipelineId, uint32_t index)
        {
            _cachedUniformLocations[index] = glGetUniformLocation(pipelineId, variable.c_str());
            return _cachedUniformLocations[index];
        }
        uint32_t GetLocationFromCache(uint32_t index) const { return _cachedUniformLocations[index]; }

    private:
        uint32_t _lastPipelineId;
        std::array<uint32_t, N> _cachedUniformLocations;
    };

    template <size_t N>
    class UniformLocationSetter
    {
    public:
        UniformLocationSetter(uint32_t pipelineId, bool recache, CachedUniformLocations<N>& cache) : _pipelineId(pipelineId), _recache(recache), _cache(cache)  {};

        void Set(const std::string& variable, bool value)
        {
            glUniform1i(NextLocation(variable), value);
        }

        void Set(const std::string& variable, int value)
        {
            glUniform1i(NextLocation(variable), value);
        }

        void Set(const std::string& variable, float value)
        {
            glUniform1f(NextLocation(variable), value);
        }

        void Set(const std::string& variable, const glm::vec2& value)
        {
            glUniform2fv(NextLocation(variable), 1, &value[0]);
        }

        void Set(const std::string& variable, const glm::vec3& value)
        {
            glUniform3fv(NextLocation(variable), 1, &value[0]);
        }

        void Set(const std::string& variable, const glm::vec4& value)
        {
            glUniform4fv(NextLocation(variable), 1, &value[0]);
        }

        void Set(const std::string& variable, const glm::mat3& value)
        {
            glUniformMatrix3fv(NextLocation(variable), 1, GL_FALSE, &value[0][0]);
        }

        void Set(const std::string& variable, const glm::mat4& value)
        {
            glUniformMatrix4fv(NextLocation(variable), 1, GL_FALSE, &value[0][0]);
        }

        void Set(const std::string& variable, const Render::Texture2DPtr& texture)
        {
            TextureManager::GetInstance().GetTexture(texture).Bind();
            glActiveTexture(GL_TEXTURE0 + _textureCounter);
            glUniform1i(NextLocation(variable), _textureCounter);
            _textureCounter++;
        }

        void SetArray(const std::string& variable, const glm::vec2* value, size_t count)
        {
            glUniform2fv(NextLocation(variable), count, &value[0][0]);
        }

        void SetArray(const std::string& variable, const glm::vec3* value, size_t count)
        {
            glUniform3fv(NextLocation(variable), count, &value[0][0]);
        }

        void SetArray(const std::string& variable, const glm::vec4* value, size_t count)
        {
            glUniform4fv(NextLocation(variable), count, &value[0][0]);
        }

        void SetArray(const std::string& variable, const glm::mat3* value, size_t count)
        {
            glUniformMatrix3fv(NextLocation(variable), count, GL_FALSE, &value[0][0][0]);
        }

        void SetArray(const std::string& variable, const glm::mat4* value, size_t count)
        {
            glUniformMatrix4fv(NextLocation(variable), count, GL_FALSE, &value[0][0][0]);
        }

    private:
        uint32_t _pipelineId;
        bool _recache;
        uint32_t _counter = 0;
        uint32_t _textureCounter = 0;
        CachedUniformLocations<N>& _cache;

        uint32_t NextLocation(const std::string& variable)
        {
            uint32_t location = _recache ? _cache.GetLocation(variable, _pipelineId, _counter) : _cache.GetLocationFromCache(_counter);
            _counter++;
            return location;
        }
    };
}