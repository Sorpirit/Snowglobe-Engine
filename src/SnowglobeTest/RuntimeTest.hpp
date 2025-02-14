#pragma once 

#include <string>
#include <iostream>
#include <vector>

#include <Engine.hpp>
#include <FileSystem.hpp>
#include <RenderSystem.hpp>
#include <Window.hpp>
#include <UISystem.hpp>

class RuntimeTest
{
public:
    virtual ~RuntimeTest() = default;

    RuntimeTest(
            Snowglobe::Engine::Engine& engine, 
            Snowglobe::Core::FileSystem& fileSystem,
            const std::string& testName) : _testName(testName), _engine(engine), _fileSystem(fileSystem) 
        {
            if(!engine.GetSystemManager()->QuerySystem<Snowglobe::Render::RenderSystem>(_renderSystem))
            {
                std::cout << "Failed to get render system" << std::endl;
            }

            _window = _renderSystem->GetMainWindow();
            _uiSystem = _renderSystem->GetUISystem();
            _input = &_window->GetInput();
        }

        virtual void Init() = 0;
        virtual void Run() = 0;

        const std::string& GetTestName() const { return _testName; }
    
protected:
        std::string _testName;

        Snowglobe::Engine::Engine& _engine;
        Snowglobe::Core::FileSystem& _fileSystem;
        Snowglobe::Render::RenderSystem* _renderSystem;
        Snowglobe::Render::Window* _window;
        Snowglobe::Core::InputReader* _input;
        Snowglobe::Render::UISystem* _uiSystem;
};

