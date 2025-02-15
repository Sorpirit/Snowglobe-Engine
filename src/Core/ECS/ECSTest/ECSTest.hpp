#pragma once

#include <iostream>
#include <unordered_map>
#include <string>

#include <glm/glm.hpp>

#include <ECS/Tag.hpp>
#include <ECS/Component.hpp>
#include <ECS/Entity.hpp>
#include <ECS/EntityData.hpp>
#include <ECS/ISystem.hpp>

REGISTER_TAG(TestTag)

namespace Snowglobe::Core::ECS::ECSTest
{

class TestComponent : public Component
{
public:
    TestComponent() = default;
    TestComponent(int testData, float testData2, std::string testData3) : _testData(testData), _testData2(testData2), _testData3(std::move(
                                                                              testData3)) {}

    int GetTestData() const { return _testData; }
    float GetTestData2() const { return _testData2; }
    const std::string& GetTestData3() const { return _testData3; }

private:
    int _testData = 0;
    float _testData2 = 0.0f;
    std::string _testData3 = "test";
};

class TransformComponent : public Component
{
public:
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;
};

class Physics2DComponent : public Component
{
public:
    Physics2DComponent() = default;
    Physics2DComponent(const glm::vec2& velocity, float mass) : Velocity(velocity), Mass(mass) {}
    
    glm::vec2 Velocity;
    float Mass;
};

class SpriteComponent : public Component
{
public:
    std::string SpriteName;
    uint32_t SpriteID = 0;
    glm::vec2 Size = glm::vec2(1.0f);

    SpriteComponent() = default;
    SpriteComponent(std::string spriteName) : SpriteName(std::move(spriteName)) {}
};

class Physics2DSystem : public ISystem
{

public:
    Physics2DSystem() { _updateOrder = UpdateOrder::Physics; }
    void Update() override
    {
        std::cout << "Physics2DSystem Update" << '\n';
        auto& entities = _entityManager->GetAllEntities();
        for (size_t i = 0; i < (entities.size() - 1); i++)
        {
            auto& entity = entities[i];
            if (entity->IsActive() == false)
                continue;

            TransformComponent* transform1 = nullptr;
            Physics2DComponent* physics1 = nullptr;
            if(entity->QueryComponents(transform1, physics1))
            {
                for (size_t j = i + 1; j < entities.size(); j++)
                {
                    auto& entity2 = entities[j];
                    if (entity2->IsActive() == false)
                        continue;

                    TransformComponent* transform2 = nullptr;
                    Physics2DComponent* physics2 = nullptr;
                    if(entity2->QueryComponents(transform2, physics2))
                    {
                        glm::vec2 distance = transform1->Position - transform2->Position;
                        float distanceLength = glm::length(distance);
                        // std::cout << "Calculating collision between: " << debug1->GetDebugString() << " and " << debug2->GetDebugString() << " Distance: " << distanceLength << '\n';
                        std::cout << "Physics1: " << physics1->Velocity.x << ", " << physics1->Velocity.y << '\n';
                        std::cout << "Physics2: " << physics2->Velocity.x << ", " << physics2->Velocity.y << '\n';
                    }
                }
            }
        }
    }
};

class RenderSystem : public ISystem
{
public:
    RenderSystem() { _updateOrder = UpdateOrder::Render; }
    void Update() override
    {
        std::cout << "RenderSystem Update" << '\n';
        auto& entities = _entityManager->GetAllEntities();
        for (auto& entity : entities)
        {
            if (entity->IsActive() == false)
                continue;

            TransformComponent* transform1 = nullptr;
            SpriteComponent* sprite1 = nullptr;
            if(entity->QueryComponents(transform1, sprite1))
            {
                std::cout << "Rendering: " << " Sprite: " << sprite1->SpriteID << " At:" << transform1->Position.x << "." << transform1->Position.y << '\n';
            }
        }

        auto specialEntities = _entityManager->GetEntitiesWithTag(1);
        for (auto& entity : specialEntities)
        {
            if (entity->IsActive() == false)
                continue;

        }
    }

private:
    uint32_t _nextSpriteID = 0;
    std::unordered_map<uint32_t, std::string> _spriteNames;
};

class TestSystemA : public ISystem
{
public:
    TestSystemA(uint32_t updateOrder, uint32_t* counter) : _counter(counter) { _updateOrder = (updateOrder); }
    void Update() override
    {
        assert(_updateOrder == *_counter && "TestSystem Update Order is incorrect");
        (*_counter)++;
    }
private:
    uint32_t* _counter;
};
class TestSystemB : public ISystem
{
public:
    TestSystemB(uint32_t updateOrder, uint32_t* counter) : ISystem(true), _counter(counter) { _updateOrder = (updateOrder); }
    void Update() override
    {
        assert(_updateOrder == *_counter && "TestSystem Update Order is incorrect");
        (*_counter)++;
    }
private:
    uint32_t* _counter;
};
class TestSystemC  : public ISystem
{
public:
    TestSystemC() = default;
    void Update() override
    {}
};

typedef MappedTupleEntityData
<
    TestComponent,
    TransformComponent,
    Physics2DComponent,
    SpriteComponent
> 
ECSTestMapEntityData;

typedef EntityManager<ECSTestMapEntityData> ECSTestEntityManager;    

}