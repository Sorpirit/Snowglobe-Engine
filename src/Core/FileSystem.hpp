#pragma once

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
    bool IsValid() const { return _isValid; }
    std::filesystem::path GetFullPath() const { return _path; }

    SnowFileHandle(const std::filesystem::path& path) : _path(path) { Initialize(path); }
    SnowFileHandle(const std::string& path) : _path(path) { Initialize(path); }
    SnowFileHandle(const char* path) : _path(path) { Initialize(path); }

  private:
    std::filesystem::path _path;
    bool _isValid;

    void Initialize(const std::filesystem::path& path);
};

class FileSystem
{
  public:
    static FileSystem& GetInstance()
    {
        static FileSystem _instance;
        return _instance;
    }

    void AddMount(const char* path);
    void AddMount(const std::string& path);
    void AddMount(const std::filesystem::path& path);

    bool TryResolvePath(const std::filesystem::path& path, std::filesystem::path& fullPath) const;

    static bool TryReadTextFile(const SnowFileHandle& handle, std::stringstream& buffer);
    static bool TryReadTextFile(const SnowFileHandle& handle, std::function<void(std::ifstream&)> read);

    static std::shared_ptr<FileTexture> LoadTexture(const SnowFileHandle& handle);

  private:
    FileSystem() = default;

    std::unordered_set<std::filesystem::path> _mounts;
};

inline void SnowFileHandle::Initialize(const std::filesystem::path& path)
{
    _isValid = FileSystem::GetInstance().TryResolvePath(path, _path);
}

inline bool FileSystem::TryResolvePath(const std::filesystem::path& path, std::filesystem::path& fullPath) const
{
    for (const auto& mount : _mounts)
    {
        std::filesystem::path tmpFullPath = mount / path;
        if (exists(tmpFullPath))
        {
            fullPath = tmpFullPath;
            return true;
        }
    }

    return false;
}

} // namespace Snowglobe::Core
