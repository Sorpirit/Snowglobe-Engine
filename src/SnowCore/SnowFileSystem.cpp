#include "SnowFileSystem.hpp"
#include <iostream>
namespace Snowglobe::SnowCore
{

SnowFileSystem::SnowFileSystem()
{

}

SnowFileSystem::~SnowFileSystem()
{

}

void SnowFileSystem::AddMount(const char *path)
{
    _mounts.insert(path);
}

void SnowFileSystem::AddMount(const std::string &path)
{
    _mounts.insert(path);
}

void SnowFileSystem::AddMount(const std::filesystem::path& path)
{
    _mounts.insert(path);
}

SnowFileHandle SnowFileSystem::CrateFileHandle(const std::string& path)
{
    return SnowFileHandle{ path };
}

char* SnowFileSystem::ReadTextFile(const SnowFileHandle& handle)
{
    auto resolvedPath = ResolvePath(handle);
    if(!std::filesystem::exists(resolvedPath))
    {
        return nullptr;
    }

    std::ifstream file(resolvedPath, std::ios::in | std::ios::binary | std::ios::ate);

    if (!file.is_open())
    {
        return nullptr;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[size + 1];

    if (file.read(buffer, size)) {
        buffer[size] = '\0'; // Null-terminate the string
        return buffer;
    } else {
        std::cerr << "Failed to read file: " << resolvedPath << std::endl;
        delete[] buffer;
        return nullptr;
    }

    return buffer;
}

std::filesystem::path SnowFileSystem::ResolvePath(const std::filesystem::path& path)
{
    for (const auto& mount : _mounts)
    {
        std::filesystem::path fullPath = mount / path;
        if (std::filesystem::exists(fullPath))
        {
            return fullPath;
        }
    }

    return path;
}

std::filesystem::path SnowFileSystem::ResolvePath(const SnowFileHandle& path)
{
    return ResolvePath(path.path);
}

    

} // namespace Snowglobe::SnowEngine
