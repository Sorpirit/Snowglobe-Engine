#pragma once

namespace Snowglobe::Render
{
    class MaterialBase
    {
    public:
        virtual ~MaterialBase() = default;
        virtual void* GetMaterialDataPtr() const = 0;
    };

    template <typename T>
    class MaterialInstance
    {
    public:
        //todo remove default constructor
        MaterialInstance() : _materialBase(nullptr), _materialData(nullptr) {}
        MaterialInstance(MaterialBase* materialBase) 
            : _materialBase(materialBase), _materialData(static_cast<T*>(materialBase->GetMaterialDataPtr()))
        {
        }

        T* Properties() const { return _materialData; }

        MaterialBase& GetMaterialBase() const { return *_materialBase; }

    private:
        MaterialBase* _materialBase;

        T* _materialData;  
    };
    
} // namespace Snowglobe::Render::Materials
