#include "ShapesShooter2DTest.hpp"

#include <numbers>

#include "BasicShapeFactory.hpp"
#include "Collider2DComponent.hpp"
#include "DependencyManager.hpp"
#include "EngineTime.hpp"
#include "LifeLinkComponent.hpp"
#include "LifetimeComponent.hpp"
#include "MeshComponent.hpp"
#include "NEdgeShape2DComponent.hpp"
#include "Physics2DComponent.hpp"
#include "TransformComponent.hpp"
#include <CommonVertexLayouts.hpp>
#include <MaterialsData/TextureColorMaterialData.hpp>

namespace Snowglobe
{

void EnemieSpawnerSystem::Update()
{
    float deltaTime = Core::EngineTime::GetDeltaEngineFrameTime();
    if (_spawnTimer <= 0)
    {
        SpawnEnemie();
        _spawnTimer = _spawnReloadDuration;
    }

    _spawnTimer -= deltaTime;
}
void EnemieSpawnerSystem::SpawnEnemie()
{
    ShapeDescription desc;
    desc.Position = glm::vec3(_enemySpawnX(gen), _enemySpawnY(gen), 0);
    desc.Velocity = glm::normalize(glm::vec2(_negativeOnePositiveOne(gen), _negativeOnePositiveOne(gen))) *
                    static_cast<float>(_enemySpeed(gen));
    desc.SideCount = static_cast<int>(_enemySides(gen));
    desc.DestroyTags = {Tags::Bullets(), Tags::Player()};
    desc.Color = glm::vec3(0.46f, 0.06f, 0.12f);
    auto enemy = ShapesShooter2DTest::CreateShape(desc, Tags::Enemies());
    enemy->AddComponent<ScoreComponent>(desc.SideCount * 10);
}

void ShapesShooter2DTest::Init()
{
    auto systemManager = _engine->GetSystemManager();

    systemManager->TryAddSystem(
        [&](const std::shared_ptr<Core::ECS::EntityManagerBase>& entityManager) {
            for (auto& entity : entityManager->GetEntitiesWithTag(Tags::Player()))
            {
                PawnInputComponent* input;
                if (entity->QueryComponent(input))
                {
                    if (_input->IsKeyHeld(Core::Key::A))
                        input->Input.x = -1;
                    else if (_input->IsKeyHeld(Core::Key::D))
                        input->Input.x = 1;
                    else
                        input->Input.x = 0;

                    if (_input->IsKeyHeld(Core::Key::S))
                        input->Input.y = -1;
                    else if (_input->IsKeyHeld(Core::Key::W))
                        input->Input.y = 1;
                    else
                        input->Input.y = 0;

                    if (glm::abs(input->Input.x) > 0 || glm::abs(input->Input.y) > 0)
                        input->Input = normalize(input->Input);
                }
            }
        },
        Core::ECS::Input, Core::ECS::DefaultLifetime, "PlayerInputSystem");

    systemManager->TryAddSystem(
        [&](const std::shared_ptr<Core::ECS::EntityManagerBase>& entityManager) {
            for (auto& entity : entityManager->GetEntitiesWithTag(Tags::Player()))
            {
                PawnInputComponent* input;
                Engine::Physics2DComponent* physics2d;
                if (entity->QueryComponents(input, physics2d))
                {
                    physics2d->Velocity += 5.0f * input->Input * Core::EngineTime::GetDeltaEngineFrameTime();
                }
            }
        },
        Core::ECS::PrePhysics, Core::ECS::DefaultLifetime, "PlayerMovementSystem");

    systemManager->TryAddSystem(
        [&](const std::shared_ptr<Core::ECS::EntityManagerBase>& entityManager) {
            for (auto& entity : entityManager->GetAllEntities())
            {
                FadeOutLifetime* fadeOut;
                Engine::LifetimeComponent* lifetime;
                Render::NEdgeShape2DComponent* shape;
                if (entity->QueryComponents(fadeOut, lifetime, shape))
                {
                    shape->Alpha = lifetime->Lifetime / lifetime->LifetimeInitial;
                }
            }
        },
        Core::ECS::PrePhysics, Core::ECS::DefaultLifetime, "FadeOutLifetimeSystem");

    systemManager->TryAddSystem(
        [&](const std::shared_ptr<Core::ECS::EntityManagerBase>& entityManager) {
            for (auto& entity : entityManager->GetAllEntities())
            {
                DestroyOnCollision* destroyOnCollision;
                Engine::Collider2DComponent* collider;
                if (!entity->QueryComponents(destroyOnCollision, collider))
                    continue;

                if (!collider->CollisionData.IsColliding)
                    continue;

                for (auto& tag : destroyOnCollision->Tags)
                {
                    if (collider->CollisionData.OtherTag == tag)
                    {
                        entity->Destroy();
                        break;
                    }
                }
            }
        },
        Core::ECS::PrePhysics, Core::ECS::DefaultLifetime, "DestroyOnCollisionSystem");

    systemManager->TryAddSystem(
        [&](const std::shared_ptr<Core::ECS::EntityManagerBase>& entityManager) {
            for (auto& entity : entityManager->GetAllEntities())
            {
                if (!entity->IsDestroyed())
                    continue;

                ExplodeOnDeath* explode;
                Core::TransformComponent* transform;
                Render::NEdgeShape2DComponent* shape;
                if (!entity->QueryComponents(explode, transform, shape))
                    continue;

                float angle = (2.0f * std::numbers::pi_v<float>) / shape->EdgeCount;
                ShapeDescription desc = {shape->EdgeCount, glm::vec3(0), glm::vec3(1)};
                desc.Scale = transform->Scale * 0.5f;
                desc.Color = shape->Color;
                desc.OutlineColor = shape->Outline;
                glm::vec3 direction;
                for (int i = 0; i < shape->EdgeCount; ++i)
                {
                    direction = glm::vec3(cos(angle * i), sin(angle * i), 0.0f);
                    desc.Position = transform->Position + direction * 0.35f;
                    desc.Velocity = direction;
                    desc.UseTriggerCollisions = true;
                    desc.ShouldExplode = false;
                    desc.DestroyTags = All;
                    auto smallShape = ShapesShooter2DTest::CreateShape(desc, Tags::EnemiesGhost());
                    smallShape->AddComponent<Engine::LifetimeComponent>(2.0f);
                    smallShape->AddComponent<FadeOutLifetime>();
                }
            }
        },
        Core::ECS::PrePhysics, Core::ECS::DefaultLifetime, "ExplodeOnDeathSystem");

    systemManager->TryAddSystem(
        [&](const std::shared_ptr<Core::ECS::EntityManagerBase>& entityManager) {
            float deltaTime = Core::EngineTime::GetDeltaEngineFrameTime();
            for (auto& entity : entityManager->GetAllEntities())
            {
                MouseControllerComponent* mouseComponent;
                Core::TransformComponent* transform;
                Engine::Physics2DComponent* physicsBody;
                if (!entity->QueryComponents(mouseComponent, transform, physicsBody))
                    continue;

                auto screenPos = _input->GetCursorScreenPosition();
                auto worldPos = _renderSystem->GetCamera().ScreenToWorld(screenPos);
                glm::vec2 cursorDirection = worldPos - transform->Position;
                cursorDirection = normalize(cursorDirection);

                float dotProd = glm::dot(physicsBody->Velocity, cursorDirection);
                if (dotProd > 7.0f)
                    continue;

                physicsBody->Velocity += cursorDirection * 30.0f * deltaTime;
            }
        },
        Core::ECS::PrePhysics, Core::ECS::DefaultLifetime, "MouseControllerSystem");

    systemManager->TryAddSystem(
        [&](const std::shared_ptr<Core::ECS::EntityManagerBase>& entityManager) {
            float deltaTime = Core::EngineTime::GetDeltaEngineFrameTime();
            for (auto& entity : entityManager->GetAllEntities())
            {
                RotationAnimationComponent* rotationAnimation;
                Core::TransformComponent* transform;
                if (!entity->QueryComponents(rotationAnimation, transform))
                    continue;

                transform->Rotation.z += rotationAnimation->RotationSpeed * deltaTime;
            }
        },
        Core::ECS::PrePhysics, Core::ECS::DefaultLifetime, "RotationAnimationSystem");

    systemManager->TryAddSystem(
        [&](const std::shared_ptr<Core::ECS::EntityManagerBase>& entityManager) {
            for (auto& entity : entityManager->GetAllEntities())
            {
                ScoreComponent* scoreComponent;
                if (entity->IsDestroyed() && entity->QueryComponent(scoreComponent))
                {
                    _score += scoreComponent->Score;
                }
            }

            std::string scoreStr = "Score: " + std::to_string(_score);
            _uiSystem->AddWorldText(glm::vec3(0, 2.3f, 0), scoreStr, glm::vec3(1.0f));
        },
        Core::ECS::PrePhysics, Core::ECS::DefaultLifetime, "ScoreSystem");

    systemManager->TryAddSystem<EnemieSpawnerSystem>(Core::ECS::DefaultLifetime, _uiSystem);

    systemManager->TryAddSystem(
        [&](const std::shared_ptr<Core::ECS::EntityManagerBase>& entityManager) { this->Run(); }, Core::ECS::PrePhysics,
        Core::ECS::DefaultLifetime, "ShapesShooter2DTest");

    gen.seed(rd());

    SpawnPlayer();
    SetupLevelBounds();
    SetupBackground();
}

void ShapesShooter2DTest::Run()
{
    float deltaTime = Core::EngineTime::GetDeltaEngineFrameTime();

    if (_player.expired())
    {
        // respawn player
        SpawnPlayer();
    }

    if (_input->IsCursorButtonHeld(Core::CursorButton::CursorButtonLeft) && _shootTimer <= 0)
    {
        Shoot();
        _shootTimer = _shootReloadDuration;
    }

    if (_input->IsCursorButtonHeld(Core::CursorButton::CursorButtonRight) && _shootSpecialTimer <= 0)
    {
        ShootSpecial();
        _shootSpecialTimer = _shootSpecialReloadDuration;
    }

    _shootTimer -= deltaTime;
    _shootSpecialTimer -= deltaTime;
}

std::shared_ptr<Core::ECS::Entity> ShapesShooter2DTest::CreateShape(const ShapeDescription& description,
                                                                    Core::ECS::Tag tag)
{
    auto manager = DI->Resolve<Engine::Engine>()->GetEntityManager();

    auto nshape = manager->CreateEntity(tag);
    nshape->AddComponent<Core::TransformComponent>(description.Position, glm::vec3(0.0f), description.Scale);
    nshape->AddComponent<Render::NEdgeShape2DComponent>(description.SideCount, description.Color,
                                                        description.OutlineColor);
    nshape->AddComponent<Engine::Physics2DComponent>(description.Velocity);
    nshape->AddComponent<Engine::Collider2DComponent>(Engine::CollisionShapeType::Circle,
                                                      description.UseTriggerCollisions);
    nshape->AddComponent<DestroyOnCollision>(description.DestroyTags);
    nshape->AddComponent<RotationAnimationComponent>(description.SpinSpeed);
    if (description.ShouldExplode)
        nshape->AddComponent<ExplodeOnDeath>();

    return nshape;
}

std::shared_ptr<Core::ECS::Entity> ShapesShooter2DTest::CreateBullet(uint32_t sides, glm::vec3 color,
                                                                     glm::vec2 position, glm::vec2 direction,
                                                                     float speed)
{
    static const std::vector bulletTags = {Tags::Default(), Tags::Player(), Tags::Enemies(), Tags::EnemiesGhost()};
    auto manager = _engine->GetEntityManager();

    auto bullet = manager->CreateEntity(Tags::Bullets());
    bullet->AddComponent<Core::TransformComponent>(glm::vec3(position + direction * 0.35f, 0), glm::vec3(0.0f),
                                                   glm::vec3(0.1f));
    bullet->AddComponent<Render::NEdgeShape2DComponent>(sides, color);
    bullet->AddComponent<Engine::Physics2DComponent>(direction * speed);
    bullet->AddComponent<Engine::Collider2DComponent>(Engine::CollisionShapeType::Circle, true);
    bullet->AddComponent<DestroyOnCollision>(bulletTags);
    bullet->AddComponent<Engine::LifetimeComponent>(5.0f);
    bullet->AddComponent<FadeOutLifetime>();
    return bullet;
}

void ShapesShooter2DTest::SpawnPlayer()
{
    ShapeDescription desc;
    desc.SideCount = 5;
    desc.Color = glm::vec3(0.04f, 0.12f, 0.23f);
    desc.DestroyTags = {Tags::Enemies()};
    auto player = CreateShape(desc, Tags::Player());
    player->SetName("Player");
    player->AddComponent<PawnInputComponent>();

    Engine::Physics2DComponent* physics;
    if (player->QueryComponent(physics))
    {
        physics->Bouncines = 0.1f;
    }

    _player = player;
}

void ShapesShooter2DTest::Shoot()
{
    Core::TransformComponent* transform;
    Engine::Physics2DComponent* physics;
    if (auto player = _player.lock())
    {
        player->QueryComponents(transform, physics);
    }

    if (transform == nullptr || physics == nullptr)
        return;

    auto screenPos = _input->GetCursorScreenPosition();
    auto worldPos = _renderSystem->GetCamera().ScreenToWorld(screenPos);
    glm::vec2 cursorDirection = worldPos - transform->Position;
    cursorDirection = normalize(cursorDirection);
    CreateBullet(16, glm::vec3(0.78f, 0.22f, 0.3f), transform->Position, cursorDirection, _bulletSpeed);

    physics->Velocity += cursorDirection * -0.3f;
}
void ShapesShooter2DTest::ShootSpecial()
{
    Core::TransformComponent* transform = nullptr;
    Engine::Physics2DComponent* physics = nullptr;
    if (auto player = _player.lock())
    {
        player->QueryComponents(transform, physics);
    }

    if (transform == nullptr)
        return;

    auto screenPos = _input->GetCursorScreenPosition();
    auto worldPos = _renderSystem->GetCamera().ScreenToWorld(screenPos);
    glm::vec2 cursorDirection = worldPos - transform->Position;
    cursorDirection = normalize(cursorDirection);
    for (int i = 0; i < 20; i++)
    {
        glm::vec3 bulletPos(_bulletDist(gen), _bulletDist(gen), 0.0f);
        bulletPos = glm::normalize(bulletPos);
        bulletPos *= _bulletRangeDist(gen);
        auto bullet = CreateBullet(16, glm::vec3(0.78f, 0.22f, 0.3f), transform->Position + bulletPos,
                                   cursorDirection * 1.3f, _bulletSpeed);
        bullet->AddComponent<MouseControllerComponent>();
    }

    physics->Velocity += cursorDirection * -0.5f;
}
void ShapesShooter2DTest::SetupLevelBounds()
{
    auto manager = _engine->GetEntityManager();
    auto materialG = _renderSystem->CreateMaterialInstance<Render::BasicShapeMaterial>();
    materialG.Properties()->color = glm::vec4(0.1f, 1.0f, 0.1f, 1.0f);

    auto wallBottom = manager->CreateEntity();
    auto wallBottomMesh = _shapeFactory.CreateShape(Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    wallBottom->SetName("wallBottom");
    wallBottom->AddComponent<Core::TransformComponent>(glm::vec3(0.0f, -2.75f, 0.0f), glm::vec3(0.0f),
                                                       glm::vec3(8.9f, 0.5f, 1.0f));
    wallBottom->AddComponent<Engine::Collider2DComponent>(Engine::CollisionShapeType::AABB);
    wallBottom->AddComponent<Engine::MeshComponent>(wallBottomMesh);
    wallBottomMesh->SetMaterial(materialG.GetMaterialBase());

    auto wallTop = manager->CreateEntity();
    auto wallTopMesh = _shapeFactory.CreateShape(Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    wallTop->SetName("wallTop");
    wallTop->AddComponent<Core::TransformComponent>(glm::vec3(0.0f, 2.75f, 0.0f), glm::vec3(0.0f),
                                                    glm::vec3(8.9f, 0.5f, 1.0f));
    wallTop->AddComponent<Engine::Collider2DComponent>(Engine::CollisionShapeType::AABB);
    wallTop->AddComponent<Engine::MeshComponent>(wallTopMesh);
    wallTopMesh->SetMaterial(materialG.GetMaterialBase());

    auto wallRight = manager->CreateEntity();
    auto wallRightMesh = _shapeFactory.CreateShape(Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    wallRight->SetName("wallRight");
    wallRight->AddComponent<Core::TransformComponent>(glm::vec3(4.695f, 0.0f, 0.0f), glm::vec3(0.0f),
                                                      glm::vec3(0.5f, 5.0f, 1.0f));
    wallRight->AddComponent<Engine::Collider2DComponent>(Engine::CollisionShapeType::AABB);
    wallRight->AddComponent<Engine::MeshComponent>(wallRightMesh);
    wallRightMesh->SetMaterial(materialG.GetMaterialBase());

    auto wallLeft = manager->CreateEntity();
    auto wallLeftMesh = _shapeFactory.CreateShape(Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    wallLeft->SetName("wallLeft");
    wallLeft->AddComponent<Core::TransformComponent>(glm::vec3(-4.695f, 0.0f, 0.0f), glm::vec3(0.0f),
                                                     glm::vec3(0.5f, 5.0f, 1.0f));
    wallLeft->AddComponent<Engine::Collider2DComponent>(Engine::CollisionShapeType::AABB);
    wallLeft->AddComponent<Engine::MeshComponent>(wallLeftMesh);
    wallLeftMesh->SetMaterial(materialG.GetMaterialBase());
}

void ShapesShooter2DTest::SetupBackground()
{
    auto backgroundFT =
        Snowglobe::Core::FileSystem::LoadTexture(Snowglobe::Core::SnowFileHandle("Textures/galaxy_background.png"));
    auto textureDesc = Snowglobe::Render::Texture2DDescriptor{
        Snowglobe::Render::TextureWrap::Repeat, Snowglobe::Render::TextureWrap::Repeat,
        Snowglobe::Render::TextureFilter::Linear, Snowglobe::Render::TextureFilter::Linear};

    auto background2D = _renderSystem->CreateTexture2D(*backgroundFT, textureDesc, "Background");
    auto backgroundMaterial =
        _renderSystem->CreateMaterialInstance<Snowglobe::Render::MaterialsData::TextureColorMaterialData>();
    backgroundMaterial.Properties()->texture = background2D;

    std::vector<Snowglobe::Render::PositionUVVertex> quad = {
        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)}, {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)},   {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)},   {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)},
    };
    auto planeVB = _renderSystem->AllocateVertexBufferPtr(quad);

    auto manager = _engine->GetEntityManager();
    auto backgroundEntity = manager->CreateEntity();
    backgroundEntity->SetName("Background");
    auto backgroundMesh = _renderSystem->CreateMeshProxy(*planeVB, "BackgroundMesh");
    backgroundMesh->SetMaterial(backgroundMaterial.GetMaterialBase());
    backgroundEntity->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(0, 0.0f, -1.0f), glm::vec3(0.0f),
                                                                        glm::vec3(9.0f, 5.0f, 1.0f));
    backgroundEntity->AddComponent<Snowglobe::Engine::MeshComponent>(backgroundMesh);
}
} // namespace Snowglobe
