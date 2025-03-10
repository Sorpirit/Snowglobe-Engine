#pragma once
#include "Asset.hpp"
#include "FileSystem.hpp"

namespace Snowglobe::Core
{
class AssetManager;
}

template<typename T>
void SetupAssetProcessor(Snowglobe::Core::AssetManager* manager);

namespace Snowglobe::Core
{
typedef std::function<void (AssetManager*, const std::filesystem::path&)> AssetProcessorFunc;

class AssetManager
{
  public:
    void ProcessAll(SnowFileHandle assetDirectory);
    template<typename T>
    void RegisterAssetProcessor()
    {
        SetupAssetProcessor<T>(this);
    }
    void RegisterAssetProcessor(const std::string& extension, AssetProcessorFunc func)
    {
        assert(!_assetProcessors.contains(extension) && "Asset processor for the exetension already registered");
        _assetProcessors[extension] = std::move(func);
        _assetProcessorOrder.push_back(extension);
    }
    void UnregisterAssetProcessor(const std::string& extension)
    {
        _assetProcessors.erase(extension);
        _assetProcessorOrder.erase(std::remove(_assetProcessorOrder.begin(), _assetProcessorOrder.end(), extension), _assetProcessorOrder.end());
    }

    template <typename T>
    void RegisterAsset(const std::string& tag, T data)
    {
        _assetsMap[tag] = std::make_unique<Asset<T>>(tag, std::move(data));
    }
    void UnregisterAsset(const std::string& tag)
    {
        auto it = _assetsMap.find(tag);
        if (it == _assetsMap.end())
            return;

        std::erase_if(_assetsTypedMap[it->second->GetAssetType()], [=](auto* asset) { return asset->GetAssetTag() == tag; });
        _assetsMap.erase(tag);
    }

    template <typename T> Asset<T>* Get(const std::string& tag)
    {
        auto it = _assetsMap.find(tag);
        if (it == _assetsMap.end())
        {
            assert(0 && "Unknown asset");
            return nullptr;
        }

        return static_cast<Asset<T>*>(it->second.get());
    }

    template <typename T> void ResolveRef(const std::string& tag, Asset<T>* assetRef)
    {
        auto it = _assetsMap.find(tag);
        if (it == _assetsMap.end())
        {
            assert(assetRef->Optional() && "Cannot find asset with tag");
            return;
        }

        assert(it->second->GetAssetType() != assetRef->GetAssetType() && "Asset type missmatch");
        assetRef->Set(*static_cast<Asset<T>*>(it->second));
    }

    void ResolveBaseRef(const std::string& tag, AssetBase* assetRef)
    {
        auto it = _assetsMap.find(tag);
        if (it == _assetsMap.end())
        {
            assert(0 && "Unknown asset");
            return;
        }

        assetRef->Set(*it->second.get());
    }

  private:

    std::unordered_map<std::string, std::unique_ptr<AssetBase>> _assetsMap;
    std::unordered_map<std::type_index, std::vector<AssetBase*>> _assetsTypedMap;
    std::unordered_map<std::string, AssetProcessorFunc> _assetProcessors;
    std::vector<std::string> _assetProcessorOrder;
};

} // namespace Snowglobe::Engine

template <>
inline void CustomPropertySerialization<Snowglobe::Core::AssetBase>(Snowglobe::Core::Serialization::SerializationAPI* api,
                                                               Snowglobe::Core::AssetBase* value, uint32_t metaFlags)
{
    std::string tag = value->GetAssetTag();
    api->BaseProperty("Tag", tag);
}

template <>
inline void CustomPropertyDeserialization<Snowglobe::Core::AssetBase>(Snowglobe::Core::Serialization::SerializationAPI* api,
                                                               Snowglobe::Core::AssetBase* value, uint32_t metaFlags)
{
    std::string tag = "";
    api->BaseProperty("Tag", tag);
    DI->Resolve<Snowglobe::Core::AssetManager>()->ResolveBaseRef(tag, value);
}