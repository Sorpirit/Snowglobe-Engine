#pragma once

#include "DependencyManager.hpp"

#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <unordered_set>

#include "FileTexture.hpp"

namespace Snowglobe::Core
{
struct SnowFileHandle
{
  public:
    [[nodiscard]] bool IsValid() const { return _isValid; }
    [[nodiscard]] std::filesystem::path GetFullPath() const { return _fullPath; }
    [[nodiscard]] std::filesystem::path GetPath() const { return _initialPath; }

    SnowFileHandle() = default;
    SnowFileHandle(const std::filesystem::path& path) : _initialPath(path) { Initialize(path); }
    SnowFileHandle(const std::string& path) : _initialPath(path) { Initialize(path); }
    SnowFileHandle(const char* path) : _initialPath(path) { Initialize(path); }

    void Initialize(const std::filesystem::path& path);

  private:
    std::filesystem::path _initialPath;
    std::filesystem::path _fullPath;
    bool _isValid = false;
};

class FileSystem
{
  public:
    FileSystem() = default;
    void AddMount(const char* path);
    void AddMount(const std::string& path);
    void AddMount(const std::filesystem::path& path);

    bool TryResolvePath(const std::filesystem::path& path, std::filesystem::path& fullPath) const;

    static bool TryReadTextFile(const SnowFileHandle& handle, std::stringstream& buffer);
    static bool TryWriteTextFile(const SnowFileHandle& handle, const std::stringstream& buffer);
    static bool TryReadTextFile(const SnowFileHandle& handle, std::function<void(std::ifstream&)> read);

    static std::shared_ptr<FileTexture> LoadTexture(const SnowFileHandle& handle);

  private:
    std::unordered_set<std::filesystem::path> _mounts;
};

inline void SnowFileHandle::Initialize(const std::filesystem::path& path)
{
    _initialPath = path;
    _isValid = DI->Resolve<FileSystem>()->TryResolvePath(_initialPath, _fullPath);
}

inline bool FileSystem::TryResolvePath(const std::filesystem::path& path, std::filesystem::path& fullPath) const
{
    for (const auto& mount : _mounts)
    {
        std::filesystem::path const tmpFullPath = mount / path;
        if (exists(tmpFullPath))
        {
            fullPath = tmpFullPath;
            return true;
        }
    }

    return false;
}

} // namespace Snowglobe::Core

namespace std
{
template <> struct hash<Snowglobe::Core::SnowFileHandle>
{
    size_t operator()(const Snowglobe::Core::SnowFileHandle& s) const
    {
        return s.IsValid() ? hash<std::filesystem::path>{}(s.GetFullPath()) : 0;
    }
};
} // namespace std
