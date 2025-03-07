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
    void LoadScene(std::string tag);
    void SaveScene(std::string tag);
    void UnloadScene(std::string tag);
private:
    Core::ECS::Lifetime _currentSceneLifetime = Core::ECS::DefaultLifetime;
};

} // namespace Snowglobe::Engine
