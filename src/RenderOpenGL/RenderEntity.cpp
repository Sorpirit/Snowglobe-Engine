#include "RenderEntity.hpp"

#include "UniformLocationSetter.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/gl.h>

namespace Snowglobe::RenderOpenGL
{

void RenderEntity::Bind(uint32_t pipelineId)
{
    UniformLocationSetter uniformSetter{pipelineId};
    uniformSetter.Set("rentity.model", GetModelMatrix());
    uniformSetter.Set("rentity.normalMatrix", _normalMatrix);
}

glm::mat4 RenderEntity::GetModelMatrix()
{
    if(_dirty)
    {
        _modelMatrix = glm::mat4(1.0f);
        _modelMatrix = glm::translate(_modelMatrix, _position);
        _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        _modelMatrix = glm::scale(_modelMatrix, _scale);

        _normalMatrix = glm::mat3(glm::transpose(glm::inverse(_modelMatrix)));
    }
    
    return _modelMatrix;
}

} // namespace Snowglobe::RenderOpenGL


