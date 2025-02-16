#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <Engine.hpp>
#include <FileSystem.hpp>
#include <RenderSystem.hpp>
#include <UISystem.hpp>
#include <Window.hpp>
#include <DependencyManager.hpp>

class RuntimeTest
{
  public:
    virtual ~RuntimeTest() = default;

    RuntimeTest(const std::string& testName)
        : _testName(testName), _engine(DI->Resolve<Snowglobe::Engine::Engine>()),
          _fileSystem(DI->Resolve<Snowglobe::Core::FileSystem>())
    {
        if (!_engine->GetSystemManager()->QuerySystem<Snowglobe::Render::RenderSystem>(_renderSystem))
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

    static std::unordered_map<std::string, std::unique_ptr<RuntimeTest>>& GetRegisteredTests()
    {
        static std::unordered_map<std::string, std::unique_ptr<RuntimeTest>> registeredTests;
        return registeredTests;
    }

    template <typename T> static void RegisterTest()
    {
        auto& map = GetRegisteredTests();
        auto test = std::make_unique<T>();
        map[test->GetTestName()] = std::move(test);
    }

    static RuntimeTest& GetTest(const std::string& name)
    {
        auto& map = GetRegisteredTests();
        return *map[name].get();
    }

  protected:
    std::string _testName;

    std::shared_ptr<Snowglobe::Engine::Engine> _engine;
    std::shared_ptr<Snowglobe::Core::FileSystem> _fileSystem;
    Snowglobe::Render::RenderSystem* _renderSystem;
    Snowglobe::Render::Window* _window;
    Snowglobe::Core::InputReader* _input;
    Snowglobe::Render::UISystem* _uiSystem;
};
