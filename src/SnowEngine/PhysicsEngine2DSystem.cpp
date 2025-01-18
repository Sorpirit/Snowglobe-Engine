#include "PhysicsEngine2DSystem.hpp"

#include "EngineTime.hpp"

namespace Snowglobe::SnowEngine
{
    void Physics2DComponent::OnDetach()
    {
        _physicsEngine.RemovePhysicsComponent(this);
    }

    void ColliderComponent::OnDetach()
    {
        _physicsEngine.RemoveCollisionComponent(this);
    }

    CollisionData ColliderComponent::CheckCollision(const ColliderComponent& collision) const
    {
        return PhysicsEngine2DSystem::Overlap(this, &collision);
    }

    PhysicsEngine2DSystem::PhysicsEngine2DSystem()
    {
    }

    PhysicsEngine2DSystem::~PhysicsEngine2DSystem()
    {
    }

    void PhysicsEngine2DSystem::Update()
    {
        float physicsDeltaTime = SnowCore::EngineTime::GetDeltaEngineFrameTime();
        for (int i = 0; i < _colliderComponents.size(); i++)
        {
            const ColliderComponent* colliderA = _colliderComponents[i].get();
            auto bodyPhysics = colliderA->GetPhysicsComponent();
            glm::vec2 position = colliderA->GetPosition();
            
            for (int j = i + 1; j < _colliderComponents.size(); j++)
            {
                
                const ColliderComponent* colliderB = _colliderComponents[j].get();
                auto collisionBodyPhysics = colliderB->GetPhysicsComponent();
                
                if (bodyPhysics == collisionBodyPhysics)
                    continue;
                
                CollisionData data = Overlap(colliderA, colliderB);

                if (!data.isColliding)
                    continue;
                
                glm::vec2 bodyVelocity = bodyPhysics != nullptr ? bodyPhysics->GetVelocity() : glm::vec2(0.0f);
                glm::vec2 collisionBodyVelocity = collisionBodyPhysics != nullptr ? collisionBodyPhysics->GetVelocity() : glm::vec2(0.0f);
                
                float bodyMass = bodyPhysics != nullptr ? bodyPhysics->GetMass() : 0.0f;
                float bodyMassInv = bodyMass > 0.0f ? 1.0f / bodyMass : 0.0f;
                float collisionBodyMass = collisionBodyPhysics != nullptr ? collisionBodyPhysics->GetMass() : 0.0f;
                float collisionBodyMassInv = collisionBodyMass > 0.0f ? 1.0f / collisionBodyMass : 0.0f;
                
                glm::vec2 relativeVelocity = collisionBodyVelocity - bodyVelocity;
                float velocityAlongNormal = glm::dot(relativeVelocity, data.normal);
                if (velocityAlongNormal > 0)
                    continue;
                
                float e = 1;
                float energy = -(1 + e) * velocityAlongNormal;
                energy /= bodyMassInv + collisionBodyMassInv;
                
                glm::vec2 impulse = energy * data.normal;
                if (bodyPhysics != nullptr)
                    bodyPhysics->SetVelocity(bodyVelocity - impulse / bodyMass);

                if (collisionBodyPhysics != nullptr)
                    collisionBodyPhysics->SetVelocity(collisionBodyVelocity + impulse / collisionBodyMass);
            }

            if (bodyPhysics != nullptr)
            {
                glm::vec2 velocity = bodyPhysics->GetVelocity();
                if (position.x <= _worldBoundaryMin.x || position.x >= _worldBoundaryMax.x)
                {
                    velocity.x = -velocity.x;
                }
            
                if (position.y <= _worldBoundaryMin.y || position.y >= _worldBoundaryMax.y)
                {
                    velocity.y = -velocity.y;
                }
            
                bodyPhysics->SetVelocity(velocity);
            }
            
        }

        for (int i = 0; i < _phyiscsComponents.size(); ++i)
        {
            Physics2DComponent* component = _phyiscsComponents[i].get();
            component->PhysicsTick(physicsDeltaTime);
            
        }
    }

    Physics2DComponent* PhysicsEngine2DSystem::AttachPhysicsComponent(SnowEntity& entity)
    {
        ColliderComponent* colliderComponent = nullptr;
        entity.QueryComponent(colliderComponent);

        auto physicsComponent = _phyiscsComponents.emplace_back(std::make_shared<Physics2DComponent>(*this, colliderComponent));
        if (colliderComponent)
            colliderComponent->SetPhyiscsComponent(physicsComponent.get());
        
        entity.AddComponent(physicsComponent.get());
        return physicsComponent.get();
    }

    ColliderComponent* PhysicsEngine2DSystem::AttachCollisionComponent(SnowEntity& entity,
        CollisionShapeType shapeType)
    {
        Physics2DComponent* physicsComponent = nullptr;
        entity.QueryComponent(physicsComponent);

        auto collisionComponent = _colliderComponents.emplace_back(std::make_shared<ColliderComponent>(*this, shapeType, physicsComponent));
        if (physicsComponent)
            physicsComponent->SetColisionComponent(collisionComponent.get());

        entity.AddComponent(collisionComponent.get());
        return collisionComponent.get();
    }

    void PhysicsEngine2DSystem::RemovePhysicsComponent(Physics2DComponent* component)
    {
        //todo: implement
    }

    void PhysicsEngine2DSystem::RemoveCollisionComponent(ColliderComponent* component)
    {
        //todo: implement
    }
} // namespace Snowglobe::SnowEngine
