#pragma once
#include "DependencyManager.hpp"
#include "ECS/Entity.hpp"
#include "Engine.hpp"

#include <memory>

namespace Snowglobe::Core
{
struct PrefabAssetData
{
    std::shared_ptr<ECS::Entity> DetachedEntity = nullptr;
};
} // namespace Snowglobe::Core