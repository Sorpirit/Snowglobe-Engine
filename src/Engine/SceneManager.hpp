#pragma once
#include "Assets/AssetManager.hpp"
#include "DependencyManager.hpp"
#include "ECS/Lifetime.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Component.hpp"

#include <memory>
#include <string>

namespace Snowglobe::Engine
{

class SceneManager
{
public:
    void LoadScene(const std::string& tag);
    void SaveScene(const std::string& tag);
    void UnloadScene(const std::string& tag);
private:
    Core::ECS::Lifetime _currentSceneLifetime = Core::ECS::DefaultLifetime;
};

} // namespace Snowglobe::Engine
