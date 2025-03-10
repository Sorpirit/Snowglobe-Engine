#pragma once
#include "FileSystem.hpp"

#include <string>

namespace Snowglobe::Core
{

class AssetBase
{
public:
    AssetBase(std::string tag, std::type_index type, bool isOptional) : _assetTag(std::move(tag)), _assetType(type), _isOptional(isOptional) {}
    virtual ~AssetBase() = default;

    [[nodiscard]] const std::string& GetAssetTag() const { return _assetTag; }
    [[nodiscard]] std::type_index GetAssetType() const { return _assetType; }
    [[nodiscard]] bool IsOptional() const { return _isOptional; }

    void Set(const AssetBase& baseAsset)
    {
        assert(baseAsset.GetAssetType() == GetAssetType() && "Asset type missmatch!");
        CopyData(baseAsset);
    }

  protected:
    std::string _assetTag = "NULL";
    std::type_index _assetType = typeid(void);
    bool _isOptional = false;

    virtual void CopyData(const AssetBase& source) = 0;
};

template <typename T, bool DataOptional = false> class Asset : public AssetBase
{
public:
    Asset() : AssetBase(std::move("NULL"), typeid(T), DataOptional) {}
    Asset(std::string tag, T data) : AssetBase(std::move(tag), typeid(T), DataOptional), _data(std::move(data)), _hasData(true) {}

    template <bool TOptional = false>
    void Set(const Asset<T, TOptional>& asset)
    {
        _assetTag = asset.GetAssetTag();
        _data = asset.GetDataValue();
        _hasData = true;
    }

    static consteval bool Optional() { return DataOptional; }

    T& GetData()
    {
        assert((HasData() || DataOptional) && "Asset data is missing!");
        return _data;
    }

    T GetDataValue() const
    {
        assert((HasData() || DataOptional) && "Asset data is missing!");
        return _data;
    }

    AssetBase& ToBase() { return *this; }
    bool HasData() const { return _hasData; }

  protected:
    void CopyData(const AssetBase& source) override
    {
        const auto& typedSource = static_cast<const Asset&>(source);
        Set(typedSource);
    }

  private:
    T _data;
    bool _hasData = false;
};

template<typename T> using AssetOptional = Asset<T, true>;

} // namespace Snowglobe::Core