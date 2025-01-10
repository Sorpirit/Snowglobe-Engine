#pragma once

#include <typeindex>
#include <typeinfo>

namespace Snowglobe::SnowCore
{

class ISnowSystem
{
protected:
    bool _requiersUpdate;
public:
    virtual ~ISnowSystem() {}
    virtual void Update() {}
    bool RequiersUpdate() const { return _requiersUpdate; }
};

class ISnowFrameSystem
{
public:
    virtual void EarlyUpdate() = 0;

    bool operator==(const ISnowFrameSystem& otherSystem) const
    {
       return typeid(this) == typeid(otherSystem);
    }

    struct HashFunction
    {
        size_t operator()(const ISnowFrameSystem& system) const
        {
            return typeid(system).hash_code();
        }
    };
};

} // namespace Snowglobe
