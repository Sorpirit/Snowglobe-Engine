#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <vector>

#include "ISnowSystem.hpp"
#include "SnowEntity.hpp"

namespace Snowglobe::SnowEngine
{
    class PhysicsEngine2DSystem;
    class Physics2DComponent;
    class ColliderComponent;

    class Physics2DComponent : public SnowEntityComponent
    {
    public:
        Physics2DComponent(PhysicsEngine2DSystem& physicsEngine) : _physicsEngine(physicsEngine) {}
        Physics2DComponent(PhysicsEngine2DSystem& physicsEngine, ColliderComponent* collisionComponent) : _physicsEngine(physicsEngine), _collisionComponent(collisionComponent) {}

        void OnDetach() override;

        glm::vec2 GetPosition() const { return _entity->GetPosition(); }
        glm::vec3 GetRotation() const { return _entity->GetRotation(); }
        glm::vec2 GetScale() const { return _entity->GetScale(); }

        glm::vec2 GetVelocity() const { return _velocity; }
        void SetVelocity(const glm::vec2& velocity) { _velocity = velocity; }

        float GetMass() const { return _mass; }
        void SetMass(float mass) { _mass = mass; }

        void PhysicsTick(float deltaTime)
        {
            glm::vec2 position = GetPosition();
            position += _velocity * deltaTime;
            _entity->SetPosition(glm::vec3(position, 0.0f));
        }

        void SetColisionComponent(ColliderComponent* collisionComponent) { _collisionComponent = collisionComponent; }
        ColliderComponent* GetCollisionComponent() const { return _collisionComponent; }
        
    private:
        glm::vec2 _velocity = glm::vec2(0.0f);
        float _mass = 1.0f;

        PhysicsEngine2DSystem& _physicsEngine;
        ColliderComponent* _collisionComponent = nullptr;
    };

    enum class CollisionShapeType
    {
        Circle,
        AABB
    };

    struct CollisionData
    {
        bool isColliding;
        glm::vec2 normal;
        float penetration;
    };

    class ColliderComponent : public SnowEntityComponent
    {
    public:
        ColliderComponent(PhysicsEngine2DSystem& physicsEngine, CollisionShapeType shapeType) : _shapeType(shapeType), _physicsEngine(physicsEngine) {}
        ColliderComponent(PhysicsEngine2DSystem& physicsEngine, CollisionShapeType shapeType, Physics2DComponent* physics2d) : _shapeType(shapeType), _physicsEngine(physicsEngine), _physics2d(physics2d) {}

        void OnDetach() override;

        glm::vec2 GetPosition() const { return _entity->GetPosition(); }
        glm::vec3 GetRotation() const { return _entity->GetRotation(); }
        glm::vec2 GetScale() const { return _entity->GetScale(); }
        
        CollisionShapeType GetShapeType() const { return _shapeType; }

        void SetPhyiscsComponent(Physics2DComponent* physics2d) { _physics2d = physics2d; }
        Physics2DComponent* GetPhysicsComponent() const { return _physics2d; }
        
        CollisionData CheckCollision(const ColliderComponent& collision) const;

        float GetRadius() const { return GetScale().x / 2.0f; }
        glm::vec2 GetMin() const { return GetPosition() - GetScale() / 2.0f; }
        glm::vec2 GetMax() const { return GetPosition() + GetScale() / 2.0f; }

    private:
        CollisionShapeType _shapeType;

        PhysicsEngine2DSystem& _physicsEngine;
        Physics2DComponent* _physics2d = nullptr;
    };

    class PhysicsEngine2DSystem : public SnowCore::ISnowSystem
    {
    public:
        PhysicsEngine2DSystem();
        ~PhysicsEngine2DSystem() override;
        void Update() override;

        Physics2DComponent* AttachPhysicsComponent(SnowEntity& entity);
        ColliderComponent* AttachCollisionComponent(SnowEntity& entity, CollisionShapeType shapeType);

        void RemovePhysicsComponent(Physics2DComponent* component);
        void RemoveCollisionComponent(ColliderComponent* component);

        static CollisionData OverlapPoint(const ColliderComponent* collider, const glm::vec2& point);
        static CollisionData Overlap(const ColliderComponent* colliderA, const ColliderComponent* colliderB);
        
    private:
        std::vector<std::shared_ptr<Physics2DComponent>> _phyiscsComponents;
        std::vector<std::shared_ptr<ColliderComponent>> _colliderComponents;

        glm::vec2 _worldBoundaryMin = glm::vec2(-4.0f, -2.5f);
        glm::vec2 _worldBoundaryMax = glm::vec2(4.0f, 2.5f);

        inline static CollisionData OverlapPointCircle(const glm::vec2& center, float radius, const glm::vec2& point);
        inline static CollisionData OverlapPointAABB(const glm::vec2& minA, const glm::vec2& maxA, const glm::vec2& point);
        inline static CollisionData OverlapCircle(const glm::vec2& centerA, float radiusA, const glm::vec2& centerB, float radiusB);
        inline static CollisionData OverlapAABB(const glm::vec2& minA, const glm::vec2& maxA, const glm::vec2& minB, const glm::vec2& maxB);
        inline static CollisionData OverlapCircleAABB(const glm::vec2& center, float radius, const glm::vec2& minA, const glm::vec2& maxA);
        
    };

    inline CollisionData PhysicsEngine2DSystem::Overlap(const ColliderComponent* colliderA, const ColliderComponent* colliderB)
    {
        if (colliderA->GetShapeType() == CollisionShapeType::Circle && colliderB->GetShapeType() == CollisionShapeType::Circle)
        {
            return OverlapCircle(colliderA->GetPosition(), colliderA->GetRadius(), colliderB->GetPosition(), colliderB->GetRadius());
        }

        if (colliderA->GetShapeType() == CollisionShapeType::AABB && colliderB->GetShapeType() == CollisionShapeType::AABB)
        {
            return OverlapAABB(colliderA->GetMin(), colliderA->GetMax(), colliderB->GetMin(), colliderB->GetMax());
        }

        if (colliderA->GetShapeType() == CollisionShapeType::Circle && colliderB->GetShapeType() == CollisionShapeType::AABB)
        {
            return OverlapCircleAABB(colliderA->GetPosition(), colliderA->GetRadius(), colliderB->GetMin(), colliderB->GetMax());
        }

        if (colliderA->GetShapeType() == CollisionShapeType::AABB && colliderB->GetShapeType() == CollisionShapeType::Circle)
        {
            auto result = OverlapCircleAABB(colliderB->GetPosition(), colliderB->GetRadius(), colliderA->GetMin(), colliderA->GetMax());
            result.normal = -result.normal;
            return result;
        }

        return CollisionData{false, glm::vec2(0.0f), 0.0f};
    }

    inline CollisionData PhysicsEngine2DSystem::OverlapPoint(const ColliderComponent* collider, const glm::vec2& point)
    {
        if (collider->GetShapeType() == CollisionShapeType::Circle)
        {
            return OverlapPointCircle(collider->GetPosition(), collider->GetScale().x, point);
        }
        else if (collider->GetShapeType() == CollisionShapeType::AABB)
        {
            return OverlapPointAABB(collider->GetPosition() - collider->GetScale() / 2.0f, collider->GetPosition() + collider->GetScale() / 2.0f, point);
        }

        return CollisionData{false, glm::vec2(0.0f), 0.0f};
    }

    inline CollisionData PhysicsEngine2DSystem::OverlapPointCircle(const glm::vec2& center, float radius,
                                                                   const glm::vec2& point)
    {
        glm::vec2 dist = point - center;
        float distSq = glm::dot(dist, dist);
        float radiusSq = radius * radius;
        if (distSq <= radiusSq)
        {
            return CollisionData{true, glm::normalize(dist), radius - glm::length(dist)};
        }

        return CollisionData{false, glm::vec2(0.0f), 0.0f};
    }

    inline CollisionData PhysicsEngine2DSystem::OverlapCircle(const glm::vec2& centerA, float radiusA,
        const glm::vec2& centerB, float radiusB)
    {
        glm::vec2 dist = centerB - centerA;
        float radiusSum = radiusA + radiusB;
        
        float distSq = glm::dot(dist, dist);
        float radiusSumSq = radiusSum * radiusSum;

        if (distSq <= radiusSumSq)
        {
            return CollisionData{true, glm::normalize(dist), radiusSum - glm::length(dist)};
        }

        return CollisionData{false, glm::vec2(0.0f), 0.0f};
    }

    inline CollisionData PhysicsEngine2DSystem::OverlapPointAABB(const glm::vec2& minA, const glm::vec2& maxA,
        const glm::vec2& point)
    {
        bool xOverlap = point.x >= minA.x && point.x <= maxA.x;
        bool yOverlap = point.y >= minA.y && point.y <= maxA.y;
        return CollisionData{xOverlap && yOverlap, glm::vec2(0.0f), 0.0f};
    }

    inline CollisionData PhysicsEngine2DSystem::OverlapAABB(const glm::vec2& minA, const glm::vec2& maxA,
        const glm::vec2& minB, const glm::vec2& maxB)
    {
        bool xOverlap = minA.x <= maxB.x && maxA.x >= minB.x;
        bool yOverlap = minA.y <= maxB.y && maxA.y >= minB.y;

        if (xOverlap && yOverlap)
        {
            glm::vec2 centerA = (minA + maxA) / 2.0f;
            glm::vec2 centerB = (minB + maxB) / 2.0f;
            
            glm::vec2 dist = centerB - centerA;
            glm::vec2 overlap = glm::vec2(std::abs(dist.x), std::abs(dist.y)); // todo: check if this is correct
            return CollisionData{true, glm::normalize(dist), std::min(overlap.x, overlap.y)};
        }
        
        return CollisionData{xOverlap && yOverlap, glm::vec2(0.0f), 0.0f};
    }

    inline CollisionData PhysicsEngine2DSystem::OverlapCircleAABB(const glm::vec2& center, float radius,
        const glm::vec2& minA, const glm::vec2& maxA)
    {
        glm::vec2 closest = glm::clamp(center, minA, maxA);
        glm::vec2 dist = closest - center;
        float distSq = glm::dot(dist, dist);
        float radiusSq = radius * radius;
        if (distSq <= radiusSq)
        {
            return CollisionData{true, glm::normalize(dist), radius - glm::length(dist)};
        }

        return CollisionData{false, glm::vec2(0.0f), 0.0f};
    }
    
} // namespace Snowglobe::SnowEngine
