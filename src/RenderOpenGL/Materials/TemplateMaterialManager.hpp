#pragma once

#include "BasicShapeFactory.hpp"
#include "IMaterialManager.hpp"
#include "MaterialBaseOpenGL.hpp"
#include <list>

namespace Snowglobe::RenderOpenGL::Materials
{
    template <typename MaterialImpl, typename MaterialData>
    class TemplateMaterialManager : public IMaterialManager
    {
        public:
            static TemplateMaterialManager<MaterialImpl, MaterialData>* GetInstance()
            {
                static TemplateMaterialManager<MaterialImpl, MaterialData> instance;
                return &instance;
            }

            MaterialBaseOpenGL* CreateMaterial(const std::string& debugName) override
            {
                _materials.push_back(MaterialImpl(debugName));
                return &_materials.back();
            }

        private:
            std::list<MaterialImpl> _materials;
    };
    
} // namespace Snowglobe::RenderOpenGL::Materials
