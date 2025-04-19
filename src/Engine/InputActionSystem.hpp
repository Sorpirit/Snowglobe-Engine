#pragma once
#include <functional>

#include "InputConstants.hpp"
#include "InputReader.hpp"
#include "SpriteRenderComponent.hpp"

namespace Snowglobe::Engine {

using InputActionCallback = std::function<void(bool isStarted)>;

class InputAction
{
public:
    // has two states STARTED and ENDED
    void Trigger(bool isStarted);
    void RegisterCallback(InputActionCallback callback);
private:
    std::vector<InputActionCallback> _callbacks;
};

class InputActionSystem
{
public:
    void Init(Core::InputReader& input);

    template <typename T>
    void RegisterInputConfig()
    {
        _inputConfig = std::make_shared<T>();
    }
    template <typename T>
    T* GetInputMapping()
    {
        auto ptr = std::static_pointer_cast<T>(_inputConfig);
        return ptr.get();
    }

    void SetMapping(Core::Key key, InputAction& action);
    void SetMapping(Core::CursorButton mouse, InputAction& action);

  private:
    std::shared_ptr<void> _inputConfig;
    std::unordered_map<Core::Key, InputActionCallback> _keyInputActions;
    std::unordered_map<Core::CursorButton, InputActionCallback> _mouseInputActions;
};

}