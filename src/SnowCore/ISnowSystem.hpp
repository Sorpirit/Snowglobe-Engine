#pragma once

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

} // namespace Snowglobe
