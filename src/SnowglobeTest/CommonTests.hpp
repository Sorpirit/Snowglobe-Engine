#pragma once

#include <string>
#include <glm/glm.hpp>

#include "RuntimeTest.hpp"

#include "EngineTime.hpp"

#include "ECS/Entity.hpp"

#include "PhysicsEngine2DSystem.hpp"
#include "BasicShapeFactory.hpp"
#include "CommonVertexLayouts.hpp"
#include "MeshComponent.hpp"
#include "SpectatorCamera.hpp"

class BaseShapeFactoryTests : public RuntimeTest
{
    public:
        BaseShapeFactoryTests(Snowglobe::Engine::Engine& engine, Snowglobe::Core::FileSystem& fileSystem) :
            RuntimeTest(engine, fileSystem, "BaseShapeFactoryTests"),
            _shapeFactory(_renderSystem),
            _gradientMaterial(_renderSystem->CreateMaterialInstance<Snowglobe::Render::BasicShapeMaterial>())
            {}
        void Init() override;
        void Run() override;

    private:
        Snowglobe::Render::BasicShapeFactory _shapeFactory;

        Snowglobe::Render::MeshProxy* _triangleMesh = nullptr;
        Snowglobe::Render::MeshProxy* _planeMesh = nullptr;
        Snowglobe::Render::MeshProxy* _diskMesh = nullptr;

        Snowglobe::Render::MaterialInstance<Snowglobe::Render::BasicShapeMaterial> _gradientMaterial;

        glm::vec2 _scale1 = glm::vec2(0.5f, 0.5f);
        glm::vec2 _scale2 = glm::vec2(2.0f, 2.0f);

        glm::vec3 _gradient1 = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 _gradient2 = glm::vec3(0.0f, 0.0f, 1.0f);
};

class UITest : public RuntimeTest
{
    public:
        UITest(
            Snowglobe::Engine::Engine& engine, 
            Snowglobe::Core::FileSystem& fileSystem) : RuntimeTest(engine, fileSystem, "UITest") {}
        void Init() override;
        void Run() override;

private:

    struct DemoEntity
    {
        std::string Name;
        glm::vec3 Position;
        glm::vec2 Velocity;
        float Mass;
        bool HasColision;
        glm::vec3 Color;
    };

    int _selectedEntity = 0;
    std::vector<DemoEntity> _entities = {
        {"Foo", glm::vec3(1, -2, 3), glm::vec2(4, -3), 1.0f, true, glm::vec3(1, 0, 0)},
        {"Bar", glm::vec3(4, 5, -6), glm::vec2(0, 0), 4.0f, false, glm::vec3(0, 1, 0)},
        {"Baz", glm::vec3(-7, 8, 9), glm::vec2(0, 0), 1.0f, true, glm::vec3(0, 0, 1)}
    };
};

class TextureTests : public RuntimeTest
{
    public:
        TextureTests(
            Snowglobe::Engine::Engine& engine, 
            Snowglobe::Core::FileSystem& fileSystem) : RuntimeTest(engine, fileSystem, "TextureTests") {}
        void Init() override;
        void Run() override;
};

class Phyiscs2DTests : public RuntimeTest
{
public:
    Phyiscs2DTests(
        Snowglobe::Engine::Engine& engine, 
        Snowglobe::Core::FileSystem& fileSystem) : RuntimeTest(engine, fileSystem, "Phyiscs2DTests"), _shapeFactory(_renderSystem) {}
    void Init() override;
    void Run() override;

private:
    Snowglobe::Render::BasicShapeFactory _shapeFactory;
};

class CameraTests : public RuntimeTest
{
public:
    CameraTests(
        Snowglobe::Engine::Engine& engine, Snowglobe::Core::FileSystem& fileSystem) :
            RuntimeTest(engine, fileSystem, "CameraTests"), _shapeFactory(_renderSystem), _spectator(_renderSystem->GetCamera(), _window->GetInput())
    {
        auto manager = _engine.GetEntityManager();
        _cubes.emplace_back(manager->CreateEntity());
        _cubes.emplace_back(manager->CreateEntity());
        _cubes.emplace_back(manager->CreateEntity());
        _cubes.emplace_back(manager->CreateEntity());
        _cubes.emplace_back(manager->CreateEntity());
    }
    
    void Init() override;
    void Run() override;
private:
    Snowglobe::Render::BasicShapeFactory _shapeFactory;
    bool _isOrthographic = false;

    Snowglobe::Engine::SpectatorCamera _spectator;

    std::vector<std::shared_ptr<Snowglobe::Core::ECS::Entity>> _cubes;
    std::vector<std::shared_ptr<Snowglobe::Engine::MeshComponent>>_cubeMeshC;

    void AddQuad(std::vector<Snowglobe::Render::PositionUVVertex>& vertices, glm::mat4x4 transform);
};


class Assigment1Tests : public RuntimeTest
{
public:
    struct ObjectDescriptor
    {
        std::string Shape;
        std::string Name;
        glm::vec3 Position;
        glm::vec3 Scale;
        glm::vec2 Velocity;
        glm::vec3 Color;
        
    };

    struct SceneDescriptor
    {
        int WindowWidth;
        int WindowHeight;
        std::string Font;
        std::vector<ObjectDescriptor> Objects;
    };

    Assigment1Tests(
        Snowglobe::Engine::Engine& engine, 
        Snowglobe::Core::FileSystem& fileSystem,
        SceneDescriptor& descriptor) : RuntimeTest(engine, fileSystem, "Assigment1Tests"), _shapeFactory(_renderSystem), _descriptor(descriptor) {}
    void Init() override;
    void Run() override;
    
    static SceneDescriptor LoadScene(Snowglobe::Core::FileSystem& fileSystem, const Snowglobe::Core::SnowFileHandle& sceneFile);
    
private:
    Snowglobe::Render::BasicShapeFactory _shapeFactory;
    SceneDescriptor& _descriptor;
};


class LightTests : public RuntimeTest
{
public:
    LightTests(
        Snowglobe::Engine::Engine& engine, Snowglobe::Core::FileSystem& fileSystem) :
            RuntimeTest(engine, fileSystem, "CameraTests"), _shapeFactory(_renderSystem), _spectator(_renderSystem->GetCamera(), _window->GetInput())
    {}
    
    void Init() override;
    void Run() override;
private:
    Snowglobe::Render::BasicShapeFactory _shapeFactory;
    bool _isOrthographic = false;

    Snowglobe::Engine::SpectatorCamera _spectator;

    std::vector<std::shared_ptr<Snowglobe::Core::ECS::Entity>> _cubes;
    std::vector<std::shared_ptr<Snowglobe::Engine::MeshComponent>>_cubeMeshC;

    void AddQuad(std::vector<Snowglobe::Render::PositionUVVertex>& vertices, glm::mat4x4 transform);
    void AddQuad(std::vector<Snowglobe::Render::PositionNormalUVVertex>& vertices, glm::mat4x4 transform);
    void AddSphere(std::vector<Snowglobe::Render::PositionNormalUVVertex>& vertices, glm::mat4x4 transform);
    
    struct SceneCube
    {
        glm::vec3 Position;
        glm::vec3 Rotation;
        glm::vec3 Scale;
        glm::vec4 Color;
    };
};