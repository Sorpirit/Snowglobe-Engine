#include "PhysicsEngine2DSystem.hpp"

#include "EngineTime.hpp"

namespace Snowglobe::SnowEngine
{
    void PhysicsEngine2DSystem::Update()
    {
        float physicsDeltaTime = SnowCore::EngineTime::GetDeltaEngineFrameTime();
        
        auto& entities = _entityManager.GetAllEntities();
        if (entities.size() <= 1)
            return;
        
        for (size_t i = 0; i < (entities.size() - 1); i++)
        {
            auto& entity1 = entities[i];
            if (entity1->IsActive() == false)
                continue;

            SnowCore::TransformComponent* tranfrom1 = nullptr;
            Collider2DComponent* collider1 = nullptr;
            if(!entity1->QueryComponents(tranfrom1, collider1))
                continue;

            for (size_t j = i + 1; j < entities.size(); j++)
            {
                auto& entity2 = entities[j];
                if (entity2->IsActive() == false)
                    continue;

                SnowCore::TransformComponent* tranfrom2 = nullptr;
                Collider2DComponent* collider2 = nullptr;
                if(!entity2->QueryComponents(tranfrom2, collider2))
                    continue;

                auto collisionData = Overlap(*tranfrom1, *collider1, *tranfrom2, *collider2);

                if (!collisionData.IsColliding)
                    continue;

                Physics2DComponent* physics1 = nullptr;
                glm::vec2 physic1Velocity = glm::vec2(0.0f);
                float physic1Mass = 0.0f;
                float physic1MassInv = 0.0f;
                
                Physics2DComponent* physics2 = nullptr;
                glm::vec2 physic2Velocity = glm::vec2(0.0f);
                float physic2Mass = 0.0f;
                float physic2MassInv = 0.0f;

                if (entity1->QueryComponent(physics1))
                {
                    physic1Velocity = physics1->Velocity;
                    physic1Mass = physics1->Mass;
                    physic1MassInv = physic1Mass > 0.0f ? 1.0f / physic1Mass : 0.0f;
                }

                if (entity2->QueryComponent(physics2))
                {
                    physic2Velocity = physics2->Velocity;
                    physic2Mass = physics2->Mass;
                    physic2MassInv = physic2Mass > 0.0f ? 1.0f / physic2Mass : 0.0f;
                }
                
                glm::vec2 relativeVelocity = physic2Velocity - physic1Velocity;
                float velocityAlongNormal = glm::dot(relativeVelocity, collisionData.Normal);
                if (velocityAlongNormal > 0)
                    continue;
                
                float e = 1;
                float energy = -(1 + e) * velocityAlongNormal;
                energy /= physic1MassInv + physic2MassInv;
                
                glm::vec2 impulse = energy * collisionData.Normal;
                if (physics1 != nullptr)
                    physics1->Velocity = physic1Velocity - impulse / physic1Mass;

                if (physics2 != nullptr)
                    physics2->Velocity = physic2Velocity + impulse / physic2Mass;
            }
        }

        for (auto& entity : entities)
        {
            if (entity->IsActive() == false)
                continue;

            SnowCore::TransformComponent* tranfrom = nullptr;
            Physics2DComponent* physics = nullptr;
            if(!entity->QueryComponents(tranfrom, physics))
                continue;

            tranfrom->Position += glm::vec3(physics->Velocity * physicsDeltaTime, 0.0f);
            tranfrom->Rotation.z += physics->AngularVelocity * physicsDeltaTime;
            
            physics->Velocity = physics->Velocity * (1 - physics->Drag);
            physics->AngularVelocity = physics->AngularVelocity * (1 - physics->AngularDrag);
        }
    }
} // namespace Snowglobe::SnowEngine
