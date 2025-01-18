#pragma once 

#include <string>
#include <iostream>
#include <vector>

#include <SnowEngine.hpp>
#include <SnowFileSystem.hpp>
#include <RenderSystem.hpp>
#include <Window.hpp>
#include <UISystem.hpp>

class RuntimeTest
{
    public:
        RuntimeTest(
            Snowglobe::SnowEngine::SnowEngine& engine, 
            Snowglobe::SnowCore::SnowFileSystem& fileSystem,
            const std::string& testName) : _testName(testName), _engine(engine), _fileSystem(fileSystem) 
        {
            if(!engine.QuerySystem<Snowglobe::Render::RenderSystem>(_renderSystem))
            {
                std::cout << "Failed to get render system" << std::endl;
            }

            _window = _renderSystem->GetMainWindow();
            _uiSystem = _renderSystem->GetUISystem();
        }

        virtual void Init() = 0;
        virtual void Run() = 0;

        const std::string& GetTestName() const { return _testName; }
    
    protected:
        std::string _testName;

        Snowglobe::SnowEngine::SnowEngine& _engine;
        Snowglobe::SnowCore::SnowFileSystem& _fileSystem;
        Snowglobe::Render::RenderSystem* _renderSystem;
        Snowglobe::Render::Window* _window;
        Snowglobe::Render::UISystem* _uiSystem;
};

