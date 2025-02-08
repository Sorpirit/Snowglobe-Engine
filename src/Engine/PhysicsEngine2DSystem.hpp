#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ECS/ISystem.hpp"
#include "ECS/Entity.hpp"

#include "TransformComponent.hpp"
#include "Collider2DComponent.hpp"

namespace Snowglobe::Engine
{
    class PhysicsEngine2DSystem : public Core::ECS::ISystem
    {
    public:
        void Update() override;

        static CollisionData OverlapPoint(const Core::TransformComponent& transform, const Collider2DComponent& collider, const glm::vec2& point);
        static CollisionData Overlap(const Core::TransformComponent& transformA, const Collider2DComponent& colliderA, const Core::TransformComponent& transformB, const Collider2DComponent& colliderB);
        
    private:
        glm::vec2 _worldBoundaryMin = glm::vec2(-4.0f, -2.5f);
        glm::vec2 _worldBoundaryMax = glm::vec2(4.0f, 2.5f);

        inline static CollisionData OverlapPointCircle(const glm::vec2& center, float radius, const glm::vec2& point);
        inline static CollisionData OverlapPointAABB(const glm::vec2& minA, const glm::vec2& maxA, const glm::vec2& point);
        inline static CollisionData OverlapCircle(const glm::vec2& centerA, float radiusA, const glm::vec2& centerB, float radiusB);
        inline static CollisionData OverlapAABB(const glm::vec2& minA, const glm::vec2& maxA, const glm::vec2& minB, const glm::vec2& maxB);
        inline static CollisionData OverlapCircleAABB(const glm::vec2& center, float radius, const glm::vec2& minA, const glm::vec2& maxA);
        
        static glm::vec2 GetMin(const Core::TransformComponent& transform) { return transform.Position - transform.Scale / 2.0f; }
        static glm::vec2 GetMax(const Core::TransformComponent& transform) { return transform.Position + transform.Scale / 2.0f; }
        static float GetRadius(const Core::TransformComponent& transform) { return transform.Scale.x / 2.0f; } 
    };

    inline CollisionData PhysicsEngine2DSystem::Overlap(const Core::TransformComponent& transformA, const Collider2DComponent& colliderA, const Core::TransformComponent& transformB, const Collider2DComponent& colliderB)
    {
        if (colliderA.ShapeType == CollisionShapeType::Circle && colliderB.ShapeType == CollisionShapeType::Circle)
        {
            return OverlapCircle(transformA.Position, GetRadius(transformA), transformB.Position, GetRadius(transformB));
        }

        if (colliderA.ShapeType == CollisionShapeType::AABB && colliderB.ShapeType == CollisionShapeType::AABB)
        {
            return OverlapAABB(GetMin(transformA), GetMax(transformA), GetMin(transformB), GetMax(transformB));
        }

        if (colliderA.ShapeType == CollisionShapeType::Circle && colliderB.ShapeType == CollisionShapeType::AABB)
        {
            return OverlapCircleAABB(transformA.Position, GetRadius(transformA), GetMin(transformB), GetMax(transformB));
        }

        if (colliderA.ShapeType == CollisionShapeType::AABB && colliderB.ShapeType == CollisionShapeType::Circle)
        {
            auto result = OverlapCircleAABB(transformB.Position, GetRadius(transformB), GetMin(transformA), GetMax(transformA));
            result.Normal = -result.Normal;
            return result;
        }

        return CollisionData{.IsColliding= false, .Normal= glm::vec2(0.0f), .Penetration= 0.0f};
    }

    inline CollisionData PhysicsEngine2DSystem::OverlapPoint(const Core::TransformComponent& transform, const Collider2DComponent& collider, const glm::vec2& point)
    {
        if (collider.ShapeType == CollisionShapeType::Circle)
        {
            return OverlapPointCircle(transform.Position, GetRadius(transform), point);
        }
        else if (collider.ShapeType == CollisionShapeType::AABB)
        {
            return OverlapPointAABB(GetMin(transform), GetMax(transform), point);
        }

        return CollisionData{.IsColliding= false, .Normal= glm::vec2(0.0f), .Penetration= 0.0f};
    }

    inline CollisionData PhysicsEngine2DSystem::OverlapPointCircle(const glm::vec2& center, float radius,
                                                                   const glm::vec2& point)
    {
        glm::vec2 dist = point - center;
        float distSq = glm::dot(dist, dist);
        float radiusSq = radius * radius;
        if (distSq <= radiusSq)
        {
            return {.IsColliding= true, .Normal = glm::normalize(dist), .Penetration= radius - glm::length(dist)};
        }

        return {.IsColliding= false, .Normal= glm::vec2(0.0f), .Penetration= 0.0f};
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
            return {.IsColliding= true, .Normal = glm::normalize(dist), .Penetration= radiusSum - glm::length(dist)};
        }

        return {.IsColliding= false, .Normal= glm::vec2(0.0f), .Penetration= 0.0f};
    }

    inline CollisionData PhysicsEngine2DSystem::OverlapPointAABB(const glm::vec2& minA, const glm::vec2& maxA,
                                                                 const glm::vec2& point)
    {
        bool xOverlap = point.x >= minA.x && point.x <= maxA.x;
        bool yOverlap = point.y >= minA.y && point.y <= maxA.y;
        return {.IsColliding= xOverlap && yOverlap, .Normal= glm::vec2(0.0f), .Penetration= 0.0f};
    }

    inline CollisionData PhysicsEngine2DSystem::OverlapAABB(const glm::vec2& minA, const glm::vec2& maxA,
                                                            const glm::vec2& minB, const glm::vec2& maxB)
    {
        bool xOverlap = minA.x <= maxB.x && maxA.x >= minB.x;
        bool yOverlap = minA.y <= maxB.y && maxA.y >= minB.y;

        if (xOverlap && yOverlap)
        {
            float overlapX = std::min(maxA.x, maxB.x) - std::max(minA.x, minB.x);
            float overlapY = std::min(maxA.y, maxB.y) - std::max(minA.y, minB.y);
            if (overlapX < overlapY)
            {
                return {
                    .IsColliding= true,
                    .Normal = glm::vec2((minA.x < minB.x) ? 1.0f : -1.0f, 0.0f),
                    .Penetration=overlapX
                };
            }
            else
            {
                return {
                    .IsColliding= true,
                    .Normal = glm::vec2(0.0f, (minA.y < minB.y) ? 1.0f : -1.0f),
                    .Penetration=overlapY
                };
            }
        }
        
        return {.IsColliding= xOverlap && yOverlap, .Normal= glm::vec2(0.0f), .Penetration= 0.0f};
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
            return {.IsColliding= true, .Normal = glm::normalize(dist), .Penetration= radius - glm::length(dist)};
        }

        return {.IsColliding= false, .Normal= glm::vec2(0.0f), .Penetration= 0.0f};
    }
}