#pragma once
#include "vector"
#include <random>

#include "BasicShapeFactory.hpp"
#include "Engine.hpp"
#include "FileSystem.hpp"
#include "RuntimeTest.hpp"
#include "TweenerSystem.hpp"
#include "ECS/Tag.hpp"

REGISTER_TAG(Player)
REGISTER_TAG(Enemies)
REGISTER_TAG(EnemiesGhost)
REGISTER_TAG(Bullets)

namespace Snowglobe
{
    
class PawnInputComponent : public Core::ECS::Component
{
public:
    glm::vec2 Input = glm::vec2(0.0f);
};
class DestroyOnCollision : public Core::ECS::Component
{
public:
    DestroyOnCollision() = default;
    DestroyOnCollision(const std::vector<Core::ECS::Tag>& tags)
        : Tags(tags)
    {}

    std::vector<Core::ECS::Tag> Tags;
};
class ExplodeOnDeath : public Core::ECS::Component { };
class FadeOutLifetime : public Core::ECS::Component { };
class MouseControllerComponent : public Core::ECS::Component { };
class RotationAnimationComponent : public Core::ECS::Component 
{
public:
    RotationAnimationComponent() = default;
    RotationAnimationComponent(float rotationSpeed) : RotationSpeed(rotationSpeed) { }

    float RotationSpeed = 1.0f;
};
class ScoreComponent : public Core::ECS::Component
{
public:
    ScoreComponent() = default;
    ScoreComponent(int score)
        : Score(score)
    {
    }

    int Score = 10;
};

class EnemieSpawnerSystem : public Core::ECS::ISystem
{
public:
    EnemieSpawnerSystem(Render::UISystem* uiSystem) : _uiSystem(uiSystem), 
        _negativeOnePositiveOne(-1.0f, 1.0f), _enemySides(3, 11), _enemySpeed(0.5f, 2.0f), _enemySpawnX(-4.0f, 4.0f), _enemySpawnY(-2.0f, 2.0f)
    {
        gen.seed(rd());
    }

    void Update() override;
private:
    void SpawnEnemie();

    float _spawnTimer = 1.0f;
    float _spawnReloadDuration = 1.0f;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> _negativeOnePositiveOne;
    std::uniform_real_distribution<> _enemySides;
    std::uniform_real_distribution<> _enemySpeed;
    std::uniform_real_distribution<> _enemySpawnX;
    std::uniform_real_distribution<> _enemySpawnY;

    Render::UISystem* _uiSystem;
};

const std::vector All = {Tags::Default(), Tags::Player(), Tags::Enemies(), Tags::Bullets(), Tags::EnemiesGhost()};
    
struct ShapeDescription
{
    int SideCount = 3;
    glm::vec3 Color = glm::vec3(0);
    glm::vec3 OutlineColor = glm::vec3(0.76f, 0.87f, 0.89f);
    glm::vec3 Position = glm::vec3(0);
    glm::vec3 Scale = glm::vec3(0.5f);
    glm::vec2 Velocity = glm::vec3(0);
    bool UseTriggerCollisions = false;
    float SpinSpeed = 30.0f;
    bool ShouldExplode = true;
    std::vector<Core::ECS::Tag> DestroyTags = All;
};
    
class ShapesShooter2DTest : public RuntimeTest
{
public:
    ShapesShooter2DTest(Engine::Engine& engine, Core::FileSystem& fileSystem);

    void Init() override;
    void Run() override;

    static std::shared_ptr<Core::ECS::Entity> CreateShape(const ShapeDescription& description, Core::ECS::Tag tag = Tags::Default());
private:
    std::weak_ptr<Core::ECS::Entity> _player;
    std::weak_ptr<Core::ECS::Entity> _debug;
    float _shootTimer = 0.0f;
    float _shootReloadDuration = 0.15f;
    float _shootSpecialTimer = 0.0f;
    float _shootSpecialReloadDuration = 5.0f;
    float _bulletSpeed = 8.0f;

    float _score = 0;

    Render::BasicShapeFactory _shapeFactory;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> _bulletDist;
    std::uniform_real_distribution<> _bulletRangeDist;
    
    std::shared_ptr<Core::ECS::Entity> CreateBullet(uint32_t sides, glm::vec3 color, glm::vec2 position, glm::vec2 direction, float speed);

    void SpawnPlayer();
    void Shoot();
    void ShootSpecial();
    void SetupLevelBounds();
    void SetupBackground();
};

} 