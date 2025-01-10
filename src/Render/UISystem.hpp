#pragma once

#include <memory>

#include <ISnowSystem.hpp>

namespace Snowglobe::Render
{
    class UISystem;

    class UIPanel
    {
    public:
        UIPanel(UISystem* system) : _system(system) {}
        ~UIPanel();

        bool Close() 
        {
            if(!_isOpen)
                return false;

            _isOpen = false; 
            return true;
        }
    private:
        UISystem* _system;
        bool _isOpen = true;
    };
    
    

    class UISystem : public SnowCore::ISnowSystem
    {
    protected:
    public:
        virtual ~UISystem() override {}

        virtual void Update() override = 0;

        virtual std::unique_ptr<UIPanel> OpenUIPanel(const char* title) = 0;
        virtual void Close(UIPanel* panel) = 0;
    };
    
} // namespace Snowglobe::Render
