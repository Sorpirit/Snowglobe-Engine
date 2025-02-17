#include "FileSystem.hpp"
#include <iostream>
namespace Snowglobe::Core
{

void FileSystem::AddMount(const char* path) { _mounts.insert(path); }

void FileSystem::AddMount(const std::string& path) { _mounts.insert(path); }

void FileSystem::AddMount(const std::filesystem::path& path) { _mounts.insert(path); }

bool FileSystem::TryReadTextFile(const SnowFileHandle& handle, std::stringstream& buffer)
{
    if (!handle.IsValid())
        return false;

    std::ifstream file(handle.GetFullPath(), std::ios::in);
    if (!file.is_open())
        return false;

    buffer << file.rdbuf();
    return true;
}

bool FileSystem::TryReadTextFile(const SnowFileHandle& handle, std::function<void(std::ifstream&)> read)
{
    if (!handle.IsValid())
        return false;

    std::ifstream file(handle.GetFullPath(), std::ios::in);
    if (!file.is_open())
        return false;

    read(file);
    return true;
}

std::shared_ptr<FileTexture> FileSystem::LoadTexture(const SnowFileHandle& handle)
{
    if (!handle.IsValid())
        return nullptr;

    return std::make_shared<FileTexture>(handle.GetFullPath());
}

} // namespace Snowglobe::Core