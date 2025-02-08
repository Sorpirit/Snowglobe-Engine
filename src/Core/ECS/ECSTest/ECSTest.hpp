#pragma once

#include <iostream>
#include <unordered_map>
#include <string>

#include <glm/glm.hpp>

#include <ECS/Component.hpp>
#include <ECS/Entity.hpp>
#include <ECS/EntityData.hpp>
#include <ECS/ISystem.hpp>

namespace Snowglobe::Core::ECS::ECSTest
{

class DebugComponent : public Component
{
public:
    DebugComponent() = default;
    DebugComponent(std::string debugString) : _debugString(std::move(debugString)) {}

    const std::string& GetDebugString() const { return _debugString; }
private:
    std::string _debugString;
};

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

class ISpriteAllocator
{
public:
    virtual ~ISpriteAllocator() = default;
    virtual uint32_t AllocateSprite(const std::string& spriteName) = 0;
    virtual glm::vec2 GetSpriteSize(uint32_t spriteID) = 0;
    virtual void DeallocateSprite(uint32_t spriteID) = 0;
};

class SpriteComponent : public Component
{
public:
    std::string SpriteName;
    uint32_t SpriteID = 0;
    glm::vec2 Size = glm::vec2(1.0f);

    SpriteComponent() = default;
    SpriteComponent(ISpriteAllocator* allocator, std::string spriteName) : SpriteName(std::move(spriteName)), _allocator(allocator) {}

private:
    ISpriteAllocator* _allocator = nullptr;

};

class Physics2DSystem : public ISystem
{

public:
    void Update() override
    {
        std::cout << "Physics2DSystem Update" << '\n';
        auto& entities = _entityManager->GetAllEntities();
        for (size_t i = 0; i < (entities.size() - 1); i++)
        {
            auto& entity = entities[i];
            if (entity->IsActive() == false)
                continue;

            DebugComponent* debug1 = nullptr;
            TransformComponent* transform1 = nullptr;
            Physics2DComponent* physics1 = nullptr;
            if(entity->QueryComponents(debug1, transform1, physics1))
            {
                for (size_t j = i + 1; j < entities.size(); j++)
                {
                    auto& entity2 = entities[j];
                    if (entity2->IsActive() == false)
                        continue;

                    DebugComponent* debug2 = nullptr;
                    TransformComponent* transform2 = nullptr;
                    Physics2DComponent* physics2 = nullptr;
                    if(entity2->QueryComponents(debug2, transform2, physics2))
                    {
                        glm::vec2 distance = transform1->Position - transform2->Position;
                        float distanceLength = glm::length(distance);
                        std::cout << "Calculating collision between: " << debug1->GetDebugString() << " and " << debug2->GetDebugString() << " Distance: " << distanceLength << '\n';
                        std::cout << "Physics1: " << physics1->Velocity.x << ", " << physics1->Velocity.y << '\n';
                        std::cout << "Physics2: " << physics2->Velocity.x << ", " << physics2->Velocity.y << '\n';
                    }
                }
            }
        }
    }
};

class RenderSystem : public ISystem, public ISpriteAllocator
{
public:
    uint32_t AllocateSprite(const std::string& spriteName) override
    {
        std::cout << "Allocating sprite: " << spriteName << '\n';
        _spriteNames[_nextSpriteID] = spriteName;
        return _nextSpriteID++;
    }
    glm::vec2 GetSpriteSize(uint32_t spriteID) override
    {
        auto it = _spriteNames.find(spriteID);
        if (it != _spriteNames.end())
        {
            std::cout << "Getting sprite size: " << it->second << '\n';
            return glm::vec2(static_cast<float>(it->second.size()));
        }

        return glm::vec2(0.0f);
    }
    void DeallocateSprite(uint32_t spriteID) override
    {
        std::cout << "Deallocate sprite: " << spriteID << '\n';
        _spriteNames.erase(spriteID);
    }

    void Update() override
    {
        std::cout << "RenderSystem Update" << '\n';
        auto& entities = _entityManager->GetAllEntities();
        for (auto& entity : entities)
        {
            if (entity->IsActive() == false)
                continue;

            DebugComponent* debug1 = nullptr;
            TransformComponent* transform1 = nullptr;
            SpriteComponent* sprite1 = nullptr;
            if(entity->QueryComponents(debug1, transform1, sprite1))
            {
                std::cout << "Rendering: " << debug1->GetDebugString() << " Sprite: " << sprite1->SpriteID << " At:" << transform1->Position.x << "." << transform1->Position.y << '\n';
            }
        }

        auto specialEntities = _entityManager->GetEntitiesByTag(1);
        for (auto& entity : specialEntities)
        {
            if (entity->IsActive() == false)
                continue;

            DebugComponent* debug1 = nullptr;
            if(entity->QueryComponents(debug1))
            {
                std::cout << "Special Update Render: " << debug1->GetDebugString() << '\n';
            }
        }
    }

private:
    uint32_t _nextSpriteID = 0;
    std::unordered_map<uint32_t, std::string> _spriteNames;
};

typedef MappedTupleEntityData
<
    DebugComponent,
    TestComponent,
    TransformComponent,
    Physics2DComponent,
    SpriteComponent
> 
ECSTestMapEntityData;

typedef EntityManager<ECSTestMapEntityData> ECSTestEntityManager;    

}