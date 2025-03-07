#pragma once
#include "FileSystem.hpp"

#include <string>

namespace Snowglobe::Core
{

class AssetBase
{
public:
    AssetBase(std::string tag, std::type_index type) : _assetTag(std::move(tag)), _assetType(type) {}
    virtual ~AssetBase() = default;

    [[nodiscard]] const std::string& GetAssetTag() const { return _assetTag; }
    [[nodiscard]] std::type_index GetAssetType() const { return _assetType; }

    void Set(AssetBase* baseAsset)
    {
        assert(baseAsset->GetAssetType() == GetAssetType() && "Asset type missmatch!");
        CopyData(baseAsset);
    }

  protected:
    std::string _assetTag = "NULL";
    std::type_index _assetType = typeid(void);

    virtual void CopyData(AssetBase* source) = 0;
};

template <typename T> class Asset : public AssetBase
{
public:
    Asset() : AssetBase(std::move("NULL"), typeid(T)) {}
    Asset(std::string tag, T data) : AssetBase(std::move(tag), typeid(T)), _data(std::move(data)) {}

    void Set(Asset* asset)
    {
        _assetTag = asset->_assetTag;
        _data = asset->_data;
    }

    T& GetData() { return _data; }

    AssetBase& ToBase() { return *this; }

  protected:
    void CopyData(AssetBase* source) override
    {
        auto typedSource = static_cast<Asset*>(source);
        Set(typedSource);
    }

  private:
    T _data;
};

} // namespace Snowglobe::Core