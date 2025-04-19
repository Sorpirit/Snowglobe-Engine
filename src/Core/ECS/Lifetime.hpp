#pragma once
#include <cstdint>

namespace Snowglobe::Core::ECS
{

struct Lifetime
{
    Lifetime() = default;
    Lifetime(uint32_t lifetime) : ID(lifetime) {}

    bool operator==(const Lifetime& other) const { return ID == other.ID; }
    bool operator!=(const Lifetime& other) const { return ID != other.ID; }

    uint32_t ID = 0;
};

const Lifetime DefaultLifetime = { 0 };

} // namespace Snowglobe::Core::ECS

template <> struct std::hash<Snowglobe::Core::ECS::Lifetime>
{
    std::size_t operator()(const Snowglobe::Core::ECS::Lifetime& tag) const noexcept
    {
        return std::hash<uint32_t>()(tag.ID);
    }
};