#pragma once

#include <string>
#include <filesystem>
#include <unordered_set>
#include <fstream>

#include "FileTexture.hpp"

namespace Snowglobe::SnowCore
{
    struct SnowFileHandle
    {
        const std::filesystem::path path;
    };

    class SnowFileSystem
    {
    public:
        ~SnowFileSystem();

        static SnowFileSystem* GetInstance()
        {
            static SnowFileSystem _instance;
            return &_instance;
        }

        void AddMount(const char* path);
        void AddMount(const std::string& path);
        void AddMount(const std::filesystem::path& path);

        SnowFileHandle CrateFileHandle(const std::string& path);
        char* ReadTextFile(const SnowFileHandle& handle);
        FileTexture* LoadTexture(const SnowFileHandle& handle);

        std::filesystem::path ResolvePath(const std::filesystem::path& path);
        std::filesystem::path ResolvePath(const SnowFileHandle& path);

    private:
        SnowFileSystem();

        std::unordered_set<std::filesystem::path> _mounts;
    };
    
} // namespace Snowglobe::SnowEngine
