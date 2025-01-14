#include <iostream>
#include <chrono>
#include <ctime> 

using time_point = std::chrono::time_point<std::chrono::system_clock>;

namespace Snowglobe::SnowCore
{
    class EngineTime
    {
    public:
        EngineTime() : _startTime(std::chrono::system_clock::now()), _lastEngineFrameTime(_startTime), _lastRenderFrameTime(_startTime)
        {
        }

        static EngineTime* GetInstance()
        {
            static EngineTime instance;
            return &instance;
        }

        void EngineTick()
        {
            auto currentTime = std::chrono::system_clock::now();
            _deltaEngineFrameTime = std::chrono::duration<double>(currentTime - _lastEngineFrameTime).count();
            _timeSinceStart = std::chrono::duration<double>(currentTime - _startTime).count();
            _lastEngineFrameTime = currentTime;
        }

        void RenderTick()
        {
            auto currentTime = std::chrono::system_clock::now();
            _deltaRenderFrameTime = std::chrono::duration<double>(currentTime - _lastRenderFrameTime).count();
            _lastRenderFrameTime = currentTime;
        }

        static double GetTimeSinceStart()
        {
            return GetInstance()->_timeSinceStart;
        }

        static double GetDeltaEngineFrameTime()
        {
            return GetInstance()->_deltaEngineFrameTime;
        }

        static double GetDeltaRenderFrameTime()
        {
            return GetInstance()->_deltaRenderFrameTime;
        }

    private:
        time_point _startTime;
        double _timeSinceStart = 0.0;

        time_point _lastEngineFrameTime;
        double _deltaEngineFrameTime = 0.0;

        time_point _lastRenderFrameTime;
        double _deltaRenderFrameTime = 0.0;

    };
    
} // namespace Snowglobe::SnowCore