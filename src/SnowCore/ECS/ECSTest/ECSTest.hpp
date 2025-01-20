#pragma once

#include <iostream>
#include <unordered_map>
#include <string>

#include <glm/glm.hpp>

#include <ECS/Component.hpp>
#include <ECS/Entity.hpp>
#include <ECS/EntityData.hpp>
#include <ECS/ISystem.hpp>

namespace Snowglobe::SnowCore::ECS::ECSTest
{

class DebugComponent : public Component
{
public:
    DebugComponent() = default;
    DebugComponent(const std::string& debugString) : _debugString(debugString) {}

    const std::string& GetDebugString() const { return _debugString; }
private:
    std::string _debugString;
};

class TestComponent : public Component
{
public:
    TestComponent() = default;
    TestComponent(int testData, float testData2, const std::string& testData3) : _testData(testData), _testData2(testData2), _testData3(testData3) {}

    void OnAttach() override
    {
        std::cout << "TestComponent attached" << std::endl;
    }

    void OnActivate() override
    {
        std::cout << "TestComponent activated" << std::endl;
    }

    void OnDeactivate() override
    {
        std::cout << "TestComponent deactivated" << std::endl;
    }

    void OnDetach() override
    {
        std::cout << "TestComponent detached" << std::endl;
    }

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
    SpriteComponent(ISpriteAllocator* allocator, const std::string& spriteName) : SpriteName(spriteName), _allocator(allocator) {}

    void OnAttach() override
    {
        Component::OnAttach();
        
        std::cout << "SpriteComponent attached" << std::endl;
        if (_allocator)
        {
            SpriteID = _allocator->AllocateSprite(SpriteName);
            Size = _allocator->GetSpriteSize(SpriteID);
        }
    }

    void OnDetach() override
    {
        Component::OnDetach();
        
        std::cout << "SpriteComponent detached" << std::endl;
        if (_allocator)
            _allocator->DeallocateSprite(SpriteID);
    }

private:
    ISpriteAllocator* _allocator = nullptr;

};

class Physics2DSystem : public ISystem
{

public:
    Physics2DSystem(EntityManagerBase& entityManager) : ISystem(entityManager) {}

    void Update() override
    {
        std::cout << "Physics2DSystem Update" << std::endl;
        auto& entities = _entityManager.GetAllEntities();
        for (size_t i = 0; i < (entities.size() - 1); i++)
        {
            auto& entity = entities[i];
            if (entity->IsActive() == false)
                continue;

            DebugComponent* debug1 = nullptr;
            TransformComponent* tranfrom1 = nullptr;
            Physics2DComponent* physics1 = nullptr;
            if(entity->QueryComponents(debug1, tranfrom1, physics1))
            {
                for (size_t j = i + 1; j < entities.size(); j++)
                {
                    auto& entity2 = entities[j];
                    if (entity2->IsActive() == false)
                        continue;

                    DebugComponent* debug2 = nullptr;
                    TransformComponent* tranfrom2 = nullptr;
                    Physics2DComponent* physics2 = nullptr;
                    if(entity2->QueryComponents(debug2, tranfrom2, physics2))
                    {
                        glm::vec2 distance = tranfrom1->Position - tranfrom2->Position;
                        float distanceLength = glm::length(distance);
                        std::cout << "Calculating colision between: " << debug1->GetDebugString() << " and " << debug2->GetDebugString() << " Distance: " << distanceLength << std::endl;
                        std::cout << "Physics1: " << physics1->Velocity.x << ", " << physics1->Velocity.y << std::endl;
                        std::cout << "Physics2: " << physics2->Velocity.x << ", " << physics2->Velocity.y << std::endl;
                    }
                }
            }
        }
    }
};

class RenderSystem : public ISystem, public ISpriteAllocator
{
public:
    RenderSystem(EntityManagerBase& entityManager) : ISystem(entityManager) {}

    uint32_t AllocateSprite(const std::string& spriteName) override
    {
        std::cout << "Allocating sprite: " << spriteName << std::endl;
        _spriteNames[_nextSpriteID] = spriteName;
        return _nextSpriteID++;
    }
    glm::vec2 GetSpriteSize(uint32_t spriteID) override
    {
        auto it = _spriteNames.find(spriteID);
        if (it != _spriteNames.end())
        {
            std::cout << "Getting sprite size: " << it->second << std::endl;
            return glm::vec2(it->second.size());
        }

        return glm::vec2(0.0f);
    }
    void DeallocateSprite(uint32_t spriteID) override
    {
        std::cout << "Deallocate sprite: " << spriteID << std::endl;
        _spriteNames.erase(spriteID);
    }

    void Update() override
    {
        std::cout << "RenderSystem Update" << std::endl;
        auto& entities = _entityManager.GetAllEntities();
        for (size_t i = 0; i < entities.size(); i++)
        {
            auto& entity = entities[i];
            if (entity->IsActive() == false)
                continue;

            DebugComponent* debug1 = nullptr;
            TransformComponent* tranfrom1 = nullptr;
            SpriteComponent* sprite1 = nullptr;
            if(entity->QueryComponents(debug1, tranfrom1, sprite1))
            {
                std::cout << "Rendering: " << debug1->GetDebugString() << " Sprite: " << sprite1->SpriteID << " At:" << tranfrom1->Position.x << "." << tranfrom1->Position.y << std::endl;
            }
        }

        auto specialEntities = _entityManager.GetEntitiesByTag(1);
        for (size_t i = 0; i < specialEntities.size(); i++)
        {
            auto& entity = specialEntities[i];
            if (entity->IsActive() == false)
                continue;

            DebugComponent* debug1 = nullptr;
            if(entity->QueryComponents(debug1))
            {
                std::cout << "Special Update Render: " << debug1->GetDebugString() << std::endl;
            }
        }
    }

private:
    uint32_t _nextSpriteID = 0;
    std::unordered_map<uint32_t, std::string> _spriteNames;
};

typedef MapEntityData
<
    DebugComponent,
    TestComponent,
    TransformComponent,
    Physics2DComponent,
    SpriteComponent
> 
ECSTestMapEntityData;

typedef EntityManager<ECSTestMapEntityData> ECSTestEntityManager;    

} // namespace Snowglobe::SnowCore::ECS::ECSTest