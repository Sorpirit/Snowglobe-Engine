#pragma once

namespace Snowglobe::Core::ECS
{

enum UpdateOrder
{
    EntityManagerUpdate = 0,
    EarlyUpdate = 10,
    Input = 50,
    PrePhysics = 100,
    Physics = 150,
    PostPhysics = 200,
    Render = 250,
    PostRender = 300,
    Final = 400,
};

}