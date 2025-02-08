#pragma once

#include <chrono>

using time_point = std::chrono::time_point<std::chrono::system_clock>;

namespace Snowglobe::Core
{

/// @brief Provides time information for the engine
class EngineTime
{
  public:
    EngineTime()
        : _startTime(std::chrono::system_clock::now()), _lastEngineFrameTime(_startTime),
          _lastRenderFrameTime(_startTime)
    {
    }

    static EngineTime* GetInstance()
    {
        static EngineTime instance;
        return &instance;
    }

    /// @brief Updates engine timer counter. Should be called only once pre frame
    void EngineTick()
    {
        auto currentTime = std::chrono::system_clock::now();
        _deltaEngineFrameTime = std::chrono::duration<double>(currentTime - _lastEngineFrameTime).count();
        _timeSinceStart = std::chrono::duration<double>(currentTime - _startTime).count();
        _lastEngineFrameTime = currentTime;
        _frameN++;
    }

    /// @brief Updates render timer counter. Should be called only once pre frame
    void RenderTick()
    {
        auto currentTime = std::chrono::system_clock::now();
        _deltaRenderFrameTime = std::chrono::duration<double>(currentTime - _lastRenderFrameTime).count();
        _lastRenderFrameTime = currentTime;
    }

    static float GetTimeSinceStart() { return static_cast<float>(GetInstance()->_timeSinceStart); }

    static float GetDeltaEngineFrameTime() { return static_cast<float>(GetInstance()->_deltaEngineFrameTime); }

    static float GetDeltaRenderFrameTime() { return static_cast<float>(GetInstance()->_deltaRenderFrameTime); }

    static double GetTimeSinceStartD() { return GetInstance()->_timeSinceStart; }

    static double GetDeltaEngineFrameTimeD() { return GetInstance()->_deltaEngineFrameTime; }

    static double GetDeltaRenderFrameTimeD() { return GetInstance()->_deltaRenderFrameTime; }

    static uint64_t GetFrameN() { return GetInstance()->_frameN; }

  private:
    time_point _startTime;
    double _timeSinceStart = 0.0;

    time_point _lastEngineFrameTime;
    double _deltaEngineFrameTime = 0.0;

    time_point _lastRenderFrameTime;
    double _deltaRenderFrameTime = 0.0;

    uint64_t _frameN = 0;
};

} // namespace Snowglobe::Core