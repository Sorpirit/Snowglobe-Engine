#pragma once

#include <string>

#include "MaterialBaseOpenGL.hpp"

namespace Snowglobe::RenderOpenGL::Materials
{
    class IMaterialManager
    {
    public:
        virtual MaterialBaseOpenGL* CreateMaterial(const std::string& debugName) = 0;
    };
    
} // namespace Snowglobe::RenderOpenGL::Materials
