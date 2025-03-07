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
                auto it = _assetProcessors.find(entry.path().extension().string());
                if (it == _assetProcessors.end())
                    continue;

                auto path = entry.path();
                it->second(this, path);
            }
        }
    }
}
} // namespace Snowglobe::Engine