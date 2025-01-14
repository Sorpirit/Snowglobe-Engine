#include "RenderEntity.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/gl.h>

namespace Snowglobe::RenderOpenGL
{

void RenderEntity::Bind(uint32_t pipelineId)
{
    auto uniformSetter = _uniformLocations.GetSetter(pipelineId);
    uniformSetter.Set("rentity.model", GetModelMatrix());
}

glm::mat4 RenderEntity::GetModelMatrix()
{
    if(_dirty)
    {
        _modelMatrix = glm::mat4(1.0f);
        _modelMatrix = glm::scale(_modelMatrix, _scale);
        _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        _modelMatrix = glm::translate(_modelMatrix, _position);
    }
    
    return _modelMatrix;
}

} // namespace Snowglobe::RenderOpenGL


