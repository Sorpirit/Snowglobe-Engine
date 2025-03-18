#include "InputActionSystem.hpp"
#include "Engine.hpp"
#include "RenderSystem.hpp"

namespace Snowglobe::Engine
{
void InputAction::Trigger(bool isStarted)
{
    for (auto& callback : _callbacks)
    {
        callback(isStarted);
    }
}
void InputAction::RegisterCallback(InputActionCallback callback) { _callbacks.push_back(callback); }
void InputActionSystem::Init(Core::InputReader& input)
{
    input.SetExternalKeyInputCallBack([&](Core::Key key, Core::KeyStatus status) {
        auto it = _keyInputActions.find(key);
        if (it == _keyInputActions.end())
            return;
        it->second(status == Core::Pressed);
    });

    input.SetExternalMouseInputCallBack([&](Core::CursorButton mouse, Core::KeyStatus status) {
        auto it = _mouseInputActions.find(mouse);
        if (it == _mouseInputActions.end())
            return;
        it->second(status == Core::Pressed);
    });
}
void InputActionSystem::SetMapping(Core::Key key, InputAction& action)
{
    _keyInputActions[key] = [&](bool isStarted) { action.Trigger(isStarted); };
}
void InputActionSystem::SetMapping(Core::CursorButton mouse, InputAction& action)
{
    _mouseInputActions[mouse] = [&](bool isStarted) { action.Trigger(isStarted); };
}
} // namespace Snowglobe::Engine