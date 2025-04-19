#include "PhysicsEngine2DSystem.hpp"

#include "DependencyManager.hpp"
#include "Engine.hpp"
#include "EngineTime.hpp"
#include "Physics2DComponent.hpp"
#include "RenderSystem.hpp"

namespace Snowglobe::Engine
{
void PhysicsEngine2DSystem::Update()
{
    float physicsDeltaTime = Core::EngineTime::GetDeltaEngineFrameTime();
    uint64_t frameN = Core::EngineTime::GetFrameN();

    auto& entities = _entityManager->GetAllEntities();
    if (entities.size() <= 1)
        return;

    for (size_t i = 0; i < (entities.size() - 1); i++)
    {
        auto& entity1 = entities[i];
        if (entity1->IsActive() == false)
            continue;

        Core::TransformComponent* transform1 = nullptr;
        Collider2DComponent* collider1 = nullptr;
        if (!entity1->QueryComponents(transform1, collider1))
            continue;

        if (collider1->CollisionData.LastFrameN != frameN)
        {
            collider1->CollisionData.LastFrameN = frameN;
            collider1->CollisionData.IsColliding = false;
        }

        for (size_t j = i + 1; j < entities.size(); j++)
        {
            auto& entity2 = entities[j];
            if (entity2->IsActive() == false)
                continue;

            Core::TransformComponent* transform2 = nullptr;
            Collider2DComponent* collider2 = nullptr;
            if (!entity2->QueryComponents(transform2, collider2))
                continue;

            if (collider2->CollisionData.LastFrameN != frameN)
            {
                collider2->CollisionData.LastFrameN = frameN;
                collider2->CollisionData.IsColliding = false;
            }

            auto collisionData = Overlap(*transform1, *collider1, *transform2, *collider2);

            if (!collisionData.IsColliding)
                continue;

            collider1->CollisionData.IsColliding = true;
            collider1->CollisionData.Other = entity2;
            collider1->CollisionData.OtherTag = entity2->GetTag();
            collider1->CollisionData.Data = collisionData;

            collider2->CollisionData.IsColliding = true;
            collider2->CollisionData.Other = entity1;
            collider2->CollisionData.OtherTag = entity1->GetTag();
            collider2->CollisionData.Data = collisionData;
            collider2->CollisionData.Data.Normal *= -1;

            if (collider1->IsTrigger || collider2->IsTrigger)
                continue;

            Physics2DComponent* physics1 = nullptr;
            glm::vec2 physic1Velocity = glm::vec2(0.0f);
            float physic1Mass = 0.0f;
            float physic1MassInv = 0.0f;
            float physic1Bounciness = 0.0f;

            Physics2DComponent* physics2 = nullptr;
            glm::vec2 physic2Velocity = glm::vec2(0.0f);
            float physic2Mass = 0.0f;
            float physic2MassInv = 0.0f;
            float physic2Bounciness = 0.0f;

            if (entity1->QueryComponent(physics1))
            {
                physic1Velocity = physics1->Velocity;
                physic1Mass = physics1->Mass;
                physic1MassInv = physic1Mass > 0.0f ? 1.0f / physic1Mass : 0.0f;
                physic1Bounciness = physics1->Bounciness;
            }

            if (entity2->QueryComponent(physics2))
            {
                physic2Velocity = physics2->Velocity;
                physic2Mass = physics2->Mass;
                physic2MassInv = physic2Mass > 0.0f ? 1.0f / physic2Mass : 0.0f;
                physic2Bounciness = physics2->Bounciness;
            }

            glm::vec2 relativeVelocity = physic2Velocity - physic1Velocity;
            float velocityAlongNormal = glm::dot(relativeVelocity, collisionData.Normal);
            if (velocityAlongNormal > 0)
                continue;

            float e = std::max(physic1Bounciness, physic2Bounciness);
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

        Core::TransformComponent* transform = nullptr;
        Physics2DComponent* physics = nullptr;
        if (!entity->QueryComponents(transform, physics))
            continue;

        transform->Position += glm::vec3(physics->Velocity * physicsDeltaTime, 0.0f);
        transform->Rotation.z += physics->AngularVelocity * physicsDeltaTime;

        physics->Velocity -= glm::vec2(0, 9.81f) * physicsDeltaTime * physics->GravityScale;
        physics->Velocity -= physics->Velocity * physics->Drag * physicsDeltaTime;
        physics->AngularVelocity = physics->AngularVelocity * (1 - physics->AngularDrag);
    }
}

void PhysicsEngine2DDebugGizmos::Init(std::shared_ptr<Core::ECS::EntityManagerBase> entityManager)
{
    ISystem::Init(entityManager);

    Render::RenderSystem* renderPtr;
    DI->Resolve<Engine>()->GetSystemManager()->QuerySystem(renderPtr);
    _gizmosAPI = renderPtr->GetGizmos();
}
void PhysicsEngine2DDebugGizmos::Update()
{
    bool useWire = true;
    for (auto& entity : _entityManager->GetAllEntities())
        {
            if (entity->IsActive() == false)
                continue;

            Core::TransformComponent* transform = nullptr;
            if (!entity->QueryComponent(transform))
                continue;

            Collider2DComponent* collider = nullptr;
            if (entity->QueryComponent(collider) && collider->IsActive())
            {
                switch (collider->ShapeType)
                {
                case CollisionShapeType::AABB:
                    if (useWire)
                        _gizmosAPI->AddQuadWire(transform->Position, transform->Scale / 2.0f, glm::vec4(1.0f, 1.0f, 0.0f, 0.2f));
                    else
                        _gizmosAPI->AddQuad(transform->Position, transform->Scale / 2.0f, glm::vec4(1.0f, 1.0f, 0.0f, 0.2f));
                    break;
                case CollisionShapeType::Circle:
                    if (useWire)
                        _gizmosAPI->AddDiskWire(transform->Position, transform->Scale.x / 2.0f, glm::vec4(1.0f, 1.0f, 0.0f, 0.2f));
                    else
                        _gizmosAPI->AddDisk(transform->Position, transform->Scale.x / 2.0f, glm::vec4(1.0f, 1.0f, 0.0f, 0.2f));
                    break;
                }
            }

            Physics2DComponent* physics = nullptr;
            if (entity->QueryComponent(physics) && physics->IsActive())
            {
                _gizmosAPI->AddVector(transform->Position, transform->Position + glm::vec3(physics->Velocity, 1.0f), glm::vec3(0, 1, 0), glm::vec4(1.0f, 0.0f, 0.0f, 0.7f));
            }
        }
    }
} // namespace Snowglobe::Engine
