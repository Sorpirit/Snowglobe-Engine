#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "IVertexLayoutDescriptor.hpp"

namespace Snowglobe::RenderOpenGL
{
    struct PositionColorVertex
    {
        glm::vec3 position;
        glm::vec3 color;
    };
    
    class PositionColorVertexDescriptor : public IVertexLayoutDescriptor
    {
    public:
        static PositionColorVertexDescriptor* GetInstance()
        {
            static PositionColorVertexDescriptor _instance;
            return &_instance;
        }

        inline virtual void SetupAtributePointers() const {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PositionColorVertex), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PositionColorVertex), (void*)offsetof(PositionColorVertex, color));
            glEnableVertexAttribArray(1);
        }
        
    private:
        PositionColorVertexDescriptor()
        {
            _suportedVertexType = typeid(PositionColorVertex);
        }
    };

} // namespace Snowglobe::RenderOpenGL
