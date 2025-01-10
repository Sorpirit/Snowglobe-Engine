#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "IVertexLayoutDescriptor.hpp"

namespace Snowglobe::RenderOpenGL
{
    struct PositionVertex
    {
        glm::vec3 position;
    };
    
    class PositionVertexDescriptor : public IVertexLayoutDescriptor
    {
    public:
        static PositionVertexDescriptor* GetInstance()
        {
            static PositionVertexDescriptor _instance;
            return &_instance;
        }

        inline virtual void SetupAtributePointers() const {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PositionVertex), (void*)0);
            glEnableVertexAttribArray(0);
        }
        
    private:
        PositionVertexDescriptor()
        {
            _suportedVertexType = typeid(PositionVertex);
        }
    };

} // namespace Snowglobe::RenderOpenGL
