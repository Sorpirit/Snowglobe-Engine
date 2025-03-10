#include "AssetManager.hpp"

#include <filesystem>
#include <queue>
#include <ranges>

namespace Snowglobe::Core
{
void AssetManager::ProcessAll(SnowFileHandle assetDirectory)
{
    using std::ranges::subrange;

    auto rootPath = assetDirectory.GetFullPath();
    std::queue<std::filesystem::path> directories;
    directories.push(rootPath);

    std::unordered_map<std::string, std::vector<std::filesystem::path>> allAssets;
    //Pre process
    while (!directories.empty())
    {
        auto path = directories.front();
        directories.pop();

        subrange DirectoryEntries{std::filesystem::directory_iterator(path)};
        for (auto entry : DirectoryEntries)
        {
            if (entry.is_directory())
                directories.push(entry.path());
            else if (entry.is_regular_file())
            {
                const auto extension = entry.path().extension().string();
                auto it = _assetProcessors.find(extension);
                if (it == _assetProcessors.end())
                    continue;

                allAssets[extension].push_back(entry.path());
            }
        }
    }

    for (const auto& extension : _assetProcessorOrder)
    {
        auto it = _assetProcessors.find(extension);
        for (const auto& path : allAssets[extension])
        {
            it->second(this, path);
        }
    }
}
} // namespace Snowglobe::Engine