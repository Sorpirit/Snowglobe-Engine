#pragma once

#include <array>
#include "glm/glm.hpp"
#include "glm/gtx/compatibility.hpp"

#include "ECS/ISystem.hpp"
#include "glm/gtx/optimum_pow.inl"

namespace Snowglobe::Engine
{

enum TweenPlay
{
    Once,
    Loop,
    PingPong
};

enum TweenEase
{
    Linear = 0,
    
    InSyine,
    OutSine,
    InOutSine,

    InCubic,
    OutCubic,
    InOutCubic,
    
    InQuad,
    OutQuad,
    InOutQuad,

    InQuart,
    OutQuart,
    InOutQuart,

    InExpo,
    OutExpo,
    InOutExpo,

    InBack,
    OutBack,
    InOutBack,
    
};

template<typename T>
struct Tween
{
    T* ValuePtr;
    T StartValue;
    T EndValue;
    float Timer;
    float Duration;
    float InvTweenDuration;
    TweenEase Ease = TweenEase::Linear;
    TweenPlay Play = TweenPlay::Once;
    int RepeatCount = -1;

    uint32_t EntityOwner = UINT32_MAX;
};

constexpr size_t POOL_SIZE = 30;
class TweenerSystem : public Core::ECS::ISystem
{
public:
    void AnimateFloat(Core::ECS::Entity& entity, float& target, float endValue, float duration, TweenEase ease = TweenEase::Linear, TweenPlay play = TweenPlay::Once, int repeatCount = -1)
    {
        EnqueuePoolItem(_floatTweenPool, _floatTweenPoolIndex, entity.GetId(), target, target, endValue, duration, ease, play, repeatCount);
        _entityTweenLink[entity.GetId()].push_back(std::make_pair(_floatTweenPoolIndex - 1, 0));
    }
    void AnimateInt(Core::ECS::Entity& entity, int& target, int endValue, float duration, TweenEase ease = TweenEase::Linear, TweenPlay play = TweenPlay::Once, int repeatCount = -1)
    {
        EnqueuePoolItem(_intTweenPool, _intTweenPoolIndex, entity.GetId(), target, target, endValue, duration, ease, play, repeatCount);
        _entityTweenLink[entity.GetId()].push_back(std::make_pair(_intTweenPoolIndex - 1, 1));
    }
    void AnimateVec2(Core::ECS::Entity& entity, glm::vec2& target, glm::vec2 endValue, float duration, TweenEase ease = TweenEase::Linear, TweenPlay play = TweenPlay::Once, int repeatCount = -1)
    {
        EnqueuePoolItem(_vec2TweenPool, _vec2TweenPoolIndex, entity.GetId(), target, target, endValue, duration, ease, play, repeatCount);
        _entityTweenLink[entity.GetId()].push_back(std::make_pair(_vec2TweenPoolIndex - 1, 2));
    }
    void AnimateVec3(Core::ECS::Entity& entity, glm::vec3& target, glm::vec3 endValue, float duration, TweenEase ease = TweenEase::Linear, TweenPlay play = TweenPlay::Once, int repeatCount = -1)
    {
        EnqueuePoolItem(_vec3TweenPool, _vec3TweenPoolIndex, entity.GetId(), target, target, endValue, duration, ease, play, repeatCount);
        _entityTweenLink[entity.GetId()].push_back(std::make_pair(_vec3TweenPoolIndex - 1, 3));
    }

    void KillAllTweens(Core::ECS::Entity& entity) {
        for (auto pair : _entityTweenLink[entity.GetId()]) {
            uint32_t tweenIndex = std::get<0>(pair);
            switch (std::get<1>(pair)) {
                case 0:
                    PopTween(_floatTweenPool, _floatTweenPoolIndex, tweenIndex);
                break;
                case 1:
                    PopTween(_intTweenPool, _intTweenPoolIndex, tweenIndex);
                break;
                case 2:
                    PopTween(_floatTweenPool, _vec2TweenPoolIndex, tweenIndex);
                break;
                case 3:
                    PopTween(_floatTweenPool, _vec3TweenPoolIndex, tweenIndex);
                break;
            default:
                break;
            }
        }

        _entityTweenLink.erase(entity.GetId());
    }

    void UpdateLate() override;
private:
    std::array<Tween<float>, POOL_SIZE> _floatTweenPool;
    size_t _floatTweenPoolIndex = 0;
    
    std::array<Tween<int>, POOL_SIZE> _intTweenPool;
    size_t _intTweenPoolIndex = 0;

    std::array<Tween<glm::vec2>, POOL_SIZE> _vec2TweenPool;
    size_t _vec2TweenPoolIndex = 0;
    
    std::array<Tween<glm::vec3>, POOL_SIZE> _vec3TweenPool;
    size_t _vec3TweenPoolIndex = 0;

    std::unordered_map<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>> _entityTweenLink;

    
    float AnimateT(float t, TweenEase ease)
    {
        constexpr float backConst1 = 1.70158f;
        constexpr float backConst2 = backConst1 * 1.525f;
        constexpr float backConst3 = backConst1 + 1.0f;
        
        switch (ease)
        {
        case TweenEase::Linear:
            return t;
        case TweenEase::InSyine:
            return 1.0f - glm::cos(glm::half_pi<float>() * t);
        case TweenEase::OutSine:
            return glm::sin(glm::half_pi<float>() * t);
        case TweenEase::InOutSine:
            return -(glm::cos(glm::pi<float>() * t) - 1) * 0.5f;
        case TweenEase::InQuad:
            return t * t;
        case TweenEase::OutQuad:
            return 1 - (1 - t) * (1 - t);
        case TweenEase::InOutQuad:
            return t < 0.5f ? 2.0f * t * t : 1 - glm::pow(-2.0f * t + 2.0f, 2.0f) * 0.5f; 
        case TweenEase::InCubic:
            return t * t * t;
        case TweenEase::OutCubic:
            return 1 - (1.0f - t) * (1.0f - t) * (1.0f - t);
        case TweenEase::InOutCubic:
            return t < 0.5f ? 4.0f * t * t * t : 1 - glm::pow(-2.0f * t + 2.0f, 3.0f) * 0.5f;
        case TweenEase::InQuart:
            return t * t * t * t;
        case TweenEase::OutQuart:
            return 1 - (1.0f - t) * (1.0f - t) * (1.0f - t) * (1.0f - t);
        case TweenEase::InOutQuart:
            return t < 0.5f ? 8.0f * t * t * t * t : 1 - glm::pow(-2.0f * t + 2.0f, 4.0f) * 0.5f;
        case TweenEase::InExpo:
            return t == 0 ? 0 : glm::pow(2.0f, 10.0f * t - 10.0f);
        case TweenEase::OutExpo:
            return t == 1 ? 1 : 1 - glm::pow(2.0f, -10.0f * t);
        case TweenEase::InOutExpo:
            return t == 0 ? 0 : t == 1 ? 1 :
                t < 0.5f
                    ? 0.5f * glm::pow(2.0f, 20.0f * t - 10)
                    : 0.5f * (2 - glm::pow(2.0f, -20.0f * t + 10));
        case TweenEase::InBack:
            return backConst3 * t * t * t - backConst1 * t * t;
        case TweenEase::OutBack:
            return 1 + backConst3 * glm::pow3(t - 1.0f) + backConst1 * glm::pow2(t - 1.0f); 
        case TweenEase::InOutBack:
            return t < 0.5f ?
                (glm::pow2(2.0f * t) * ((backConst2 + 1) * 2 * t - backConst2)) * 0.5f :
                (glm::pow2(2.0f * t - 2.0f) * ((backConst2 + 1) * (2 * t - 2.0f) + backConst2) + 2.0f) * 0.5f;
        }

        return t;
    }
    
    template <typename TTween, typename TValue>
    void EnqueuePoolItem(std::array<TTween, POOL_SIZE>& pool, size_t& lastIndexPtr, uint32_t entityID,
        TValue& target, TValue startValue, TValue endValue, float duration, TweenEase ease = TweenEase::Linear, TweenPlay play = TweenPlay::Once, int repeatCount = -1)
    {
        assert(lastIndexPtr < POOL_SIZE && "Pool overflow, TransformTweenSystem::AnimateFloat");
        auto& value = pool[lastIndexPtr++];
        value.ValuePtr = &target;
        value.StartValue = startValue;
        value.EndValue = endValue;
        value.Duration = duration;
        value.Timer = duration;
        value.InvTweenDuration = 1.0f / duration;
        value.Ease = ease;
        value.Play = play;
        value.RepeatCount = repeatCount;

        value.EntityOwner = entityID;
    }
    
    template <typename TTween>
    void UpdateTweenPool(std::array<TTween, POOL_SIZE>& pool, size_t& lastIndexPtr, float deltaTime)
    {
        float t = 0;
        for (size_t i = lastIndexPtr; i > 0; --i)
        {
            auto& tween = pool[i - 1];
            if (tween.Timer <= 0)
            {
                switch (tween.Play)
                {
                case TweenPlay::PingPong:
                    std::swap(tween.StartValue, tween.EndValue);
                case TweenPlay::Loop:
                    if (tween.RepeatCount > 0 || tween.RepeatCount == -1)
                    {
                        *tween.ValuePtr = tween.StartValue;
                        tween.Timer = tween.Duration;
                        if (tween.RepeatCount > 0)
                        {
                            --tween.RepeatCount;
                            continue;
                        }
                    }
                    break;
                case TweenPlay::Once:
                    break;
                }

                std::erase_if(_entityTweenLink[tween.EntityOwner], [&](auto& pair) { return std::get<0>(pair) == (i - 1); });
                PopTween(pool, lastIndexPtr, i - 1);
                continue;
            }

            if (tween.ValuePtr == nullptr)
            {
                if (i != lastIndexPtr && lastIndexPtr > 1)
                    std::swap(pool[i - 1], pool[lastIndexPtr - 1]);
                
                lastIndexPtr--;
            }
            
            tween.Timer -= deltaTime;
            if (tween.Timer <= 0)
            {
                t = 1.0f;
            }
            else
            {
                t = 1.0f - tween.Timer * tween.InvTweenDuration;
                t = glm::clamp(t, 0.0f, 1.0f);
                t = AnimateT(t, tween.Ease);
                //t = glm::clamp(t, 0.0f, 1.0f);
            }
            
            //*tween.ValuePtr = glm::mix(tween.StartValue, tween.EndValue, t);
        }
    }

    template <typename TTween>
    void PopTween(std::array<TTween, POOL_SIZE>& pool, size_t& lastIndexPtr, uint32_t tweenIndex)
    {
        if (tweenIndex != (lastIndexPtr - 1) && lastIndexPtr > 1)
            std::swap(pool[tweenIndex], pool[lastIndexPtr - 1]);

        lastIndexPtr--;
    }
};

}
