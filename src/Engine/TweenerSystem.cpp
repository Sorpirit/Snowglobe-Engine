#include "TweenerSystem.hpp"

#include "EngineTime.hpp"

namespace Snowglobe::Engine
{
    void TweenerSystem::Update()
    {
        float deltaTime = Core::EngineTime::GetDeltaEngineFrameTime();
        
        if (_floatTweenPoolIndex > 0)
            UpdateTweenPool(_floatTweenPool, _floatTweenPoolIndex, deltaTime);

        if (_intTweenPoolIndex > 0)
            UpdateTweenPool(_intTweenPool, _intTweenPoolIndex, deltaTime);

        if (_vec2TweenPoolIndex > 0)
            UpdateTweenPool(_vec2TweenPool, _vec2TweenPoolIndex, deltaTime);

        if (_vec3TweenPoolIndex > 0)
            UpdateTweenPool(_vec3TweenPool, _vec3TweenPoolIndex, deltaTime);
    }
}
