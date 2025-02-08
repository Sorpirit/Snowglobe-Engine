#include "CommonTests.hpp"

#include <iostream>
#include <functional>

#include <RenderSystem.hpp>

#include <BasicShapeFactory.hpp>

#include "CommonVertexLayouts.hpp"
#include "ComponentEditor.hpp"
#include "MeshComponent.hpp"
#include "NEdgeShape2DComponent.hpp"
#include "../RenderOpenGL/LightsManager.hpp"
#include "../RenderOpenGL/MeshOpenGL.hpp"
#include "../RenderOpenGL/OpenGLRenderSystem.hpp"
#include "MaterialsData/TextureColorMaterialData.hpp"
#include "MaterialsData/TextureLitMaterialData.hpp"

void BaseShapeFactoryTests::Init()
{
    _triangleMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Triangle, glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(1.0f));
    _planeMesh =  _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    _diskMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Disk, glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(1.0f));

    auto yellowMaterial = _renderSystem->CreateMaterialInstance<Snowglobe::Render::BasicShapeMaterial>();
    yellowMaterial.Properties()->color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

    _triangleMesh->SetMaterial(yellowMaterial.GetMaterialBase());
    _planeMesh->SetMaterial(yellowMaterial.GetMaterialBase());
    _diskMesh->SetMaterial(_gradientMaterial.GetMaterialBase());

    auto e = _engine.GetEntityManager();
    auto nshape = e->CreateEntity();
    nshape->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    nshape->AddComponent<Snowglobe::Render::NEdgeShape2DComponent>(3);
    
}

void BaseShapeFactoryTests::Run()
{
    float time = Snowglobe::Core::EngineTime::GetTimeSinceStart();

    float x = (sin(time) + 1.0f) * 0.5f;
    
    _triangleMesh->SetRotation(glm::vec3(0.0f, 0.0f, x * 360));
    _triangleMesh->SetScale(glm::vec3(
        std::lerp(_scale1.x, _scale2.x, x),
        std::lerp(_scale1.y, _scale2.y, x),
        1.0f));
    
    _planeMesh->SetPosition(glm::vec3(cos(x * 2.0f * 3.14159f), sin(x * 2.0f * 3.14159f), 0.0f));
    _planeMesh->SetRotation(glm::vec3(0.0f, 0.0f, x * 360));

    _gradientMaterial.Properties()->color = glm::vec4(
        std::lerp(_gradient1.x, _gradient2.x, x),
        std::lerp(_gradient1.y, _gradient2.y, x),
        std::lerp(_gradient1.z, _gradient2.z, x),
        1.0f);

    _diskMesh->SetScale(glm::vec3(
        std::lerp(_scale1.x, _scale2.x, x),
        std::lerp(_scale1.y, _scale2.y, x),
        1.0f));
}

void UITest::Init()
{
}

void UITest::Run()
{
    {
        auto panel = _uiSystem->OpenUIPanel("Property Demo Editor");
        _uiSystem->Text("Chose entity");

        std::vector<std::string> entityNames;
        for(auto& entity : _entities)
        {
            entityNames.push_back(entity.Name);
        }
        _uiSystem->Combo("Select Entity", &_selectedEntity, entityNames);

        auto& selectedEntity = _entities[_selectedEntity];

        char buffer[128];
        strncpy_s(buffer, selectedEntity.Name.c_str(), _TRUNCATE);
        if (_uiSystem->Input("Name", buffer, 128))
        {
            selectedEntity.Name = buffer;
        }

        _uiSystem->Drag("Position", &selectedEntity.Position, 0.1f);
        _uiSystem->Slider("Velocity", &selectedEntity.Velocity, -10.0f, 10.0f);
        _uiSystem->Slider("Mass", &selectedEntity.Mass, 0.1f, 10.0f);
        _uiSystem->Checkbox("Has Colision", &selectedEntity.HasColision);
        _uiSystem->Color("Color", &selectedEntity.Color);
        _uiSystem->ToolTip("Use collisions");
        if (_uiSystem->Button("Reset Position ans Velocity"))
        {
            selectedEntity.Position = glm::vec3(0.0f);
            selectedEntity.Velocity = glm::vec2(0.0f);
        }
        _uiSystem->ToolTip("Reset the position and velocity of the entity");
        _uiSystem->Text("Same line");
        _uiSystem->SameLine();
        _uiSystem->Text("Text");
    }

    _uiSystem->AddWorldText(glm::vec3(0.0f), "Hello, Im world text", glm::vec3(1.0f, 0.0f, 0.0f));
    _uiSystem->AddWorldText(glm::vec3(2.0f, -2.0f, 0.0f), "My name", glm::vec3(1.0f, 1.0f, 0.0f));
    
}

void TextureTests::Init()
{
    auto tileTexture = Snowglobe::Core::FileSystem::LoadTexture(Snowglobe::Core::SnowFileHandle("Textures/wall.jpg"));
    auto containerTexture = Snowglobe::Core::FileSystem::LoadTexture(Snowglobe::Core::SnowFileHandle("Textures/container.jpg"));
    auto awesomeTexture = Snowglobe::Core::FileSystem::LoadTexture(Snowglobe::Core::SnowFileHandle("Textures/awesomeface.png"));
    
    auto texture = _renderSystem->CreateTexture2D(*tileTexture, {
        Snowglobe::Render::TextureWrap::Repeat,
        Snowglobe::Render::TextureWrap::Repeat,
        Snowglobe::Render::TextureFilter::Nearest,
        Snowglobe::Render::TextureFilter::Nearest});
    
    std::vector<Snowglobe::Render::PositionUVVertex> vertices = {
        {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(-0.5f, -0.5f)},
        {glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2(1.5f, -0.5f)},
        {glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(1.5f, 1.5f)},
        {glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(-0.5f, 1.5f)}
    };

    std::vector<uint32_t> indices = {
        0, 1, 2,
        0, 2, 3
    };

    auto vertexBuffer = _renderSystem->AllocateVertexBufferPtr(vertices);
    auto indexBuffer = _renderSystem->AllocateIndexBufferPtr(indices);

    auto mesh = _renderSystem->CreateMeshProxy(*vertexBuffer, *indexBuffer, "TextureQuad");
    auto textureMaterial = _renderSystem->CreateMaterialInstance<Snowglobe::Render::MaterialsData::TextureColorMaterialData>();
    textureMaterial.Properties()->texture = texture;
    mesh->SetMaterial(textureMaterial.GetMaterialBase());
}

void TextureTests::Run()
{
}

void Phyiscs2DTests::Init()
{
    auto materialL = _renderSystem->CreateMaterialInstance<Snowglobe::Render::BasicShapeMaterial>();
    materialL.Properties()->color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    auto materialR = _renderSystem->CreateMaterialInstance<Snowglobe::Render::BasicShapeMaterial>();
    materialR.Properties()->color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    auto materialG = _renderSystem->CreateMaterialInstance<Snowglobe::Render::BasicShapeMaterial>();
    materialG.Properties()->color = glm::vec4(0.1f, 1.0f, 0.1f, 1.0f);

    auto manager = _engine.GetEntityManager();

    auto ballL = manager->CreateEntity();
    auto ballLMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Disk, glm::vec3(0.0f), glm::vec3(1.0f));
    ballL->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(-3.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    ballL->AddComponent<Snowglobe::Engine::Physics2DComponent>(glm::vec2(1.0f, 0.2f), 0.0f, 1.0f, 0.0f, 1.0f);
    ballL->AddComponent<Snowglobe::Engine::Collider2DComponent>(Snowglobe::Engine::CollisionShapeType::Circle);
    ballL->AddComponent<Snowglobe::Engine::MeshComponent>(ballLMesh);
    ballL->AddComponent<Snowglobe::Engine::BaseComponentMaterial>(ballLMesh, &materialL.GetMaterialBase());
    ballL->AddComponent<Snowglobe::Render::NEdgeShape2DComponent>(3);
    ballLMesh->SetMaterial(materialL.GetMaterialBase());

    auto ballR = manager->CreateEntity();
    auto ballRMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Disk, glm::vec3(0.0f), glm::vec3(1.0f));
    ballR->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(4.0f, 0.5f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    ballR->AddComponent<Snowglobe::Engine::Physics2DComponent>(glm::vec2(-1.0f, -0.2f), 0.0f, 1.0f, 0.0f, 1.0f);
    ballR->AddComponent<Snowglobe::Engine::Collider2DComponent>(Snowglobe::Engine::CollisionShapeType::Circle);
    ballR->AddComponent<Snowglobe::Engine::MeshComponent>(ballRMesh);
    ballR->AddComponent<Snowglobe::Engine::BaseComponentMaterial>(ballRMesh, &materialR.GetMaterialBase());
    ballRMesh->SetMaterial(materialR.GetMaterialBase());

    auto planeL = manager->CreateEntity();
    auto planeLMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    planeL->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(-4.0f, -0.5f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    planeL->AddComponent<Snowglobe::Engine::Physics2DComponent>(glm::vec2(-2.0f, 0.0f), 0.0f, 1.0f, 0.0f, 1.0f);
    planeL->AddComponent<Snowglobe::Engine::Collider2DComponent>(Snowglobe::Engine::CollisionShapeType::AABB);
    planeL->AddComponent<Snowglobe::Engine::MeshComponent>(planeLMesh);
    planeL->AddComponent<Snowglobe::Engine::BaseComponentMaterial>(planeLMesh, &materialL.GetMaterialBase());
    planeLMesh->SetMaterial(materialL.GetMaterialBase());

    auto planeR = manager->CreateEntity();
    auto planeRMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    planeR->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(3.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    planeR->AddComponent<Snowglobe::Engine::Physics2DComponent>(glm::vec2(-2.0f, 0.0f), 0.0f, 1.0f, 0.0f, 1.0f);
    planeR->AddComponent<Snowglobe::Engine::Collider2DComponent>(Snowglobe::Engine::CollisionShapeType::AABB);
    planeR->AddComponent<Snowglobe::Engine::MeshComponent>(planeRMesh);
    planeR->AddComponent<Snowglobe::Engine::BaseComponentMaterial>(planeRMesh, &materialR.GetMaterialBase());
    planeRMesh->SetMaterial(materialR.GetMaterialBase());

    auto groundWall = manager->CreateEntity();
    auto groundWallMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    groundWall->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(4.0f, 0.5f, 1.0f));
    groundWall->AddComponent<Snowglobe::Engine::Collider2DComponent>(Snowglobe::Engine::CollisionShapeType::AABB);
    groundWall->AddComponent<Snowglobe::Engine::MeshComponent>(groundWallMesh);
    groundWall->AddComponent<Snowglobe::Engine::BaseComponentMaterial>(groundWallMesh, &materialG.GetMaterialBase());
    groundWallMesh->SetMaterial(materialG.GetMaterialBase());

    auto wallBottom = manager->CreateEntity();
    auto wallBottomMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    wallBottom->SetName("wallBottom");
    wallBottom->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(0.0f, -2.5f, 0.0f), glm::vec3(0.0f), glm::vec3(8.5f, 0.5f, 1.0f));
    wallBottom->AddComponent<Snowglobe::Engine::Collider2DComponent>(Snowglobe::Engine::CollisionShapeType::AABB);
    wallBottom->AddComponent<Snowglobe::Engine::MeshComponent>(wallBottomMesh);
    wallBottomMesh->SetMaterial(materialG.GetMaterialBase());

    auto wallTop = manager->CreateEntity();
    auto wallTopMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    wallTop->SetName("wallTop");
    wallTop->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(0.0f, 2.5f, 0.0f), glm::vec3(0.0f), glm::vec3(8.5f, 0.5f, 1.0f));
    wallTop->AddComponent<Snowglobe::Engine::Collider2DComponent>(Snowglobe::Engine::CollisionShapeType::AABB);
    wallTop->AddComponent<Snowglobe::Engine::MeshComponent>(wallTopMesh);
    wallTopMesh->SetMaterial(materialG.GetMaterialBase());
    
    auto wallRight = manager->CreateEntity();
    auto wallRightMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    wallRight->SetName("wallRight");
    wallRight->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(4.5f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f, 5.0f, 1.0f));
    wallRight->AddComponent<Snowglobe::Engine::Collider2DComponent>(Snowglobe::Engine::CollisionShapeType::AABB);
    wallRight->AddComponent<Snowglobe::Engine::MeshComponent>(wallRightMesh);
    wallRightMesh->SetMaterial(materialG.GetMaterialBase());

    auto wallLeft = manager->CreateEntity();
    auto wallLeftMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    wallLeft->SetName("wallLeft");
    wallLeft->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(-4.5f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f, 5.0f, 1.0f));
    wallLeft->AddComponent<Snowglobe::Engine::Collider2DComponent>(Snowglobe::Engine::CollisionShapeType::AABB);
    wallLeft->AddComponent<Snowglobe::Engine::MeshComponent>(wallLeftMesh);
    wallLeftMesh->SetMaterial(materialG.GetMaterialBase());
}

void Phyiscs2DTests::Run()
{
}

void CameraTests::Init()
{
    auto containerTexture = Snowglobe::Core::FileSystem::LoadTexture(Snowglobe::Core::SnowFileHandle("Textures/container.jpg"));
    
    auto texture = _renderSystem->CreateTexture2D(*containerTexture, {
        Snowglobe::Render::TextureWrap::Repeat,
        Snowglobe::Render::TextureWrap::Repeat,
        Snowglobe::Render::TextureFilter::Linear,
        Snowglobe::Render::TextureFilter::Linear});
    
    std::vector<Snowglobe::Render::PositionUVVertex> cubeVertices;
    AddQuad(cubeVertices, glm::mat4x4(1.0f));
    AddQuad(cubeVertices, glm::rotate(glm::mat4x4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    AddQuad(cubeVertices, glm::rotate(glm::mat4x4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    AddQuad(cubeVertices, glm::rotate(glm::mat4x4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    AddQuad(cubeVertices, glm::rotate(glm::mat4x4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    AddQuad(cubeVertices, glm::rotate(glm::mat4x4(1.0f), glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    auto cubeVB = _renderSystem->AllocateVertexBufferPtr(cubeVertices);
    
    auto containerMaterial = _renderSystem->CreateMaterialInstance<Snowglobe::Render::MaterialsData::TextureColorMaterialData>();
    containerMaterial.Properties()->texture = texture;
    containerMaterial.Properties()->color = glm::vec4(1.0f);
    

    for (int i = 0; i < _cubes.size(); ++i)
    {
        auto mesh = _renderSystem->CreateMeshProxy(*cubeVB, "CubeMesh");
        mesh->SetMaterial(containerMaterial.GetMaterialBase());
        _cubes[i]->AddComponent<Snowglobe::Core::TransformComponent>();
        _cubes[i]->AddComponent<Snowglobe::Engine::MeshComponent>(mesh);
        
    }

    Snowglobe::Core::TransformComponent* transform = nullptr;
    if (_cubes[0]->QueryComponent(transform))
    {
        transform->Position = glm::vec3(3, 1, -1);
        transform->Rotation = glm::vec3(14, 0, 25);
    }

    if (_cubes[1]->QueryComponent(transform))
    {
        transform->Position = glm::vec3(-2, 0, -6);
        transform->Rotation = glm::vec3(72, 0, 34);
    }

    if (_cubes[2]->QueryComponent(transform))
    {
        transform->Position = glm::vec3(0, -1, -4);
        transform->Rotation = glm::vec3(25, 0, 52);
    }

    if (_cubes[3]->QueryComponent(transform))
    {
        transform->Position = glm::vec3(-2, -2, -2);
        transform->Rotation = glm::vec3(50, 0, 39);
    }

    if (_cubes[4]->QueryComponent(transform))
    {
        transform->Position = glm::vec3(3, -3, -5);
        transform->Rotation = glm::vec3(87, 0, 23);
    }
    
    _renderSystem->GetCamera().SetMode(Snowglobe::Render::CameraMode::Perspective);
    _isOrthographic = false;
}

void CameraTests::Run()
{
    _spectator.Update();
    
    if (_window->GetInput().IsKeyPressed(Snowglobe::Core::Key::T))
    {
        _isOrthographic = !_isOrthographic;
        _renderSystem->GetCamera().SetMode(_isOrthographic ? Snowglobe::Render::CameraMode::Orthographic : Snowglobe::Render::CameraMode::Perspective);
    }

    float timeDelta = Snowglobe::Core::EngineTime::GetDeltaEngineFrameTime();
    for (int i = 0; i < _cubes.size(); ++i)
    {
        Snowglobe::Core::TransformComponent* transform = nullptr;
        if (!_cubes[i]->QueryComponent(transform))
            continue;
        
        auto cubeRotation = transform->Rotation;
        cubeRotation.y += 45.0f * timeDelta;
        transform->Rotation = cubeRotation;
    }
}

void CameraTests::AddQuad(std::vector<Snowglobe::Render::PositionUVVertex>& vertices, glm::mat4x4 transform)
{
    const Snowglobe::Render::PositionUVVertex quad[6] = {
        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)},
    };

    for (int i = 0; i < 6; ++i)
    {
        vertices.push_back({transform * glm::vec4(quad[i].position, 1.0f), quad[i].uv});
    }
}

void Assigment1Tests::Init()
{
    _uiSystem->SetDefaultFont(Snowglobe::Core::SnowFileHandle(_descriptor.Font));
    
    auto materialG = _renderSystem->CreateMaterialInstance<Snowglobe::Render::BasicShapeMaterial>();
    materialG.Properties()->color = glm::vec4(0.4f, 0.7f, 0.4f, 1.0f);

    auto manager = _engine.GetEntityManager();

    for (auto& desc : _descriptor.Objects)
    {
        auto entity = manager->CreateEntity();
        auto shape = desc.Shape == "Circle" ? Snowglobe::Render::BasicShape::Disk : Snowglobe::Render::BasicShape::Plane;
        auto collisionShape = desc.Shape == "Circle" ? Snowglobe::Engine::CollisionShapeType::Circle : Snowglobe::Engine::CollisionShapeType::AABB;
        auto ballLMesh = _shapeFactory.CreateShape(shape, glm::vec3(0.0f), glm::vec3(1.0f));
        auto materialL = _renderSystem->CreateMaterialInstance<Snowglobe::Render::BasicShapeMaterial>();
        materialL.Properties()->color = glm::vec4(desc.Color, 1.0f);

        entity->SetName(desc.Name);
        entity->AddComponent<Snowglobe::Core::TransformComponent>(desc.Position, glm::vec3(0), glm::vec3(desc.Scale));
        entity->AddComponent<Snowglobe::Engine::Physics2DComponent>(desc.Velocity, 0.0f, 1.0f, 0.0f, 1.0f);
        entity->AddComponent<Snowglobe::Engine::Collider2DComponent>(collisionShape);
        entity->AddComponent<Snowglobe::Engine::MeshComponent>(ballLMesh);
        entity->AddComponent<Snowglobe::Engine::BaseComponentMaterial>(ballLMesh, &materialL.GetMaterialBase());
        ballLMesh->SetMaterial(materialL.GetMaterialBase());
    }

    
    auto wallBottom = manager->CreateEntity();
    auto wallBottomMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    wallBottom->SetName("wallBottom");
    wallBottom->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(0.0f, -2.5f, 0.0f), glm::vec3(0.0f), glm::vec3(8.5f, 0.5f, 1.0f));
    wallBottom->AddComponent<Snowglobe::Engine::Collider2DComponent>(Snowglobe::Engine::CollisionShapeType::AABB);
    wallBottom->AddComponent<Snowglobe::Engine::MeshComponent>(wallBottomMesh);
    wallBottomMesh->SetMaterial(materialG.GetMaterialBase());

    auto wallTop = manager->CreateEntity();
    auto wallTopMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    wallTop->SetName("wallTop");
    wallTop->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(0.0f, 2.5f, 0.0f), glm::vec3(0.0f), glm::vec3(8.5f, 0.5f, 1.0f));
    wallTop->AddComponent<Snowglobe::Engine::Collider2DComponent>(Snowglobe::Engine::CollisionShapeType::AABB);
    wallTop->AddComponent<Snowglobe::Engine::MeshComponent>(wallTopMesh);
    wallTopMesh->SetMaterial(materialG.GetMaterialBase());
    
    auto wallRight = manager->CreateEntity();
    auto wallRightMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    wallRight->SetName("wallRight");
    wallRight->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(4.5f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f, 5.0f, 1.0f));
    wallRight->AddComponent<Snowglobe::Engine::Collider2DComponent>(Snowglobe::Engine::CollisionShapeType::AABB);
    wallRight->AddComponent<Snowglobe::Engine::MeshComponent>(wallRightMesh);
    wallRightMesh->SetMaterial(materialG.GetMaterialBase());

    auto wallLeft = manager->CreateEntity();
    auto wallLeftMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    wallLeft->SetName("wallLeft");
    wallLeft->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(-4.5f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f, 5.0f, 1.0f));
    wallLeft->AddComponent<Snowglobe::Engine::Collider2DComponent>(Snowglobe::Engine::CollisionShapeType::AABB);
    wallLeft->AddComponent<Snowglobe::Engine::MeshComponent>(wallLeftMesh);
    wallLeftMesh->SetMaterial(materialG.GetMaterialBase());
}

void Assigment1Tests::Run()
{
}

Assigment1Tests::SceneDescriptor Assigment1Tests::LoadScene(Snowglobe::Core::FileSystem& fileSystem,const Snowglobe::Core::SnowFileHandle& sceneFile)
{
    SceneDescriptor descriptor;
    fileSystem.TryReadTextFile(sceneFile, [&](std::ifstream& stream)
    {
        std::string line;
        if (stream >> line)
        {
            stream >> descriptor.WindowWidth >> descriptor.WindowHeight >> descriptor.Font;
        }
        
        while (std::getline(stream, line))
        {
            if (line.empty())
                continue;
            
            std::istringstream iss(line);
            std::string shape;
            std::string name;
            glm::vec3 position(0.0f);
            glm::vec3 scale(1.0f);
            glm::vec2 velocity(0.0f);
            glm::vec3 color;
            

            iss >> shape >> name >> position.x >> position.y >> velocity.x >> velocity.y >> color.x >> color.y >> color.z;

            if (shape == "Circle")
            {
                iss >> scale.x;
                scale.y = scale.x;
            }
            else if (shape == "Rect")
            {
                iss >> scale.x >> scale.y;
            }
            else
            {
                std::cerr << "Unknown shape: " << shape << std::endl;
                continue;
            }

            descriptor.Objects.push_back({shape, name, position, scale, velocity, color});
        } 
    });

    return descriptor;
}

void LightTests::Init()
{
    auto containerFileTexture = Snowglobe::Core::FileSystem::LoadTexture(Snowglobe::Core::SnowFileHandle("Textures/container.jpg"));
    auto diffuseFileTexture = Snowglobe::Core::FileSystem::LoadTexture(Snowglobe::Core::SnowFileHandle("Textures/container2.png"));
    auto specularFileTexture = Snowglobe::Core::FileSystem::LoadTexture(Snowglobe::Core::SnowFileHandle("Textures/container2_specular.png"));
    auto emissiveFileTexture = Snowglobe::Core::FileSystem::LoadTexture(Snowglobe::Core::SnowFileHandle("Textures/container2_emissive.png"));

    auto textureDesc = Snowglobe::Render::Texture2DDescriptor{
        Snowglobe::Render::TextureWrap::Repeat,
        Snowglobe::Render::TextureWrap::Repeat,
        Snowglobe::Render::TextureFilter::Linear,
        Snowglobe::Render::TextureFilter::Linear};
    
    // auto containerTexture2D = _renderSystem->CreateTexture2D(*containerFileTexture, textureDesc, "Container");
    auto diffuseTexture2D = _renderSystem->CreateTexture2D(*diffuseFileTexture, textureDesc, "Diffuse");
    auto specularTexture2D = _renderSystem->CreateTexture2D(*specularFileTexture, textureDesc, "Specular");
    auto emissivityTexture2D = _renderSystem->CreateTexture2D(*emissiveFileTexture, textureDesc, "Emissive");
    
    std::vector<Snowglobe::Render::PositionUVVertex> cubeVertices;
    AddQuad(cubeVertices, glm::mat4x4(1.0f));
    AddQuad(cubeVertices, glm::rotate(glm::mat4x4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    AddQuad(cubeVertices, glm::rotate(glm::mat4x4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    AddQuad(cubeVertices, glm::rotate(glm::mat4x4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    AddQuad(cubeVertices, glm::rotate(glm::mat4x4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    AddQuad(cubeVertices, glm::rotate(glm::mat4x4(1.0f), glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    auto cubeVB = _renderSystem->AllocateVertexBufferPtr(cubeVertices);

    std::vector<Snowglobe::Render::PositionNormalUVVertex> cubeVerticesNormals;
    AddQuad(cubeVerticesNormals, glm::mat4x4(1.0f));
    AddQuad(cubeVerticesNormals, glm::rotate(glm::mat4x4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    AddQuad(cubeVerticesNormals, glm::rotate(glm::mat4x4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    AddQuad(cubeVerticesNormals, glm::rotate(glm::mat4x4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    AddQuad(cubeVerticesNormals, glm::rotate(glm::mat4x4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    AddQuad(cubeVerticesNormals, glm::rotate(glm::mat4x4(1.0f), glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    auto cubeNormalVB = _renderSystem->AllocateVertexBufferPtr(cubeVerticesNormals);

    std::vector<Snowglobe::Render::PositionNormalUVVertex> sphereVerticesNormals;
    AddSphere(sphereVerticesNormals, glm::mat4x4(1.0f));
    auto sphereNormalVB = _renderSystem->AllocateVertexBufferPtr(sphereVerticesNormals);
    
    auto containerMaterial = _renderSystem->CreateMaterialInstance<Snowglobe::Render::MaterialsData::TextureLitMaterialData>();
    // containerMaterial.Properties()->texture = diffuseTexture2D;
    containerMaterial.Properties()->diffuseTexture = diffuseTexture2D;
    containerMaterial.Properties()->specularTexture = specularTexture2D;
    containerMaterial.Properties()->emissiveTexture = emissivityTexture2D;
    containerMaterial.Properties()->specularPower = 32;

    auto lightMaterial = _renderSystem->CreateMaterialInstance<Snowglobe::Render::MaterialsData::TextureColorMaterialData>();
    // lightMaterial.Properties()->texture = diffuseTexture2D;
    lightMaterial.Properties()->color = glm::vec4(1.0f);

    auto manager = _engine.GetEntityManager();
    auto pointLight = manager->CreateEntity(Snowglobe::Tags::Lights());
    pointLight->SetName("PointLight" );
    auto pointLightMesh = _renderSystem->CreateMeshProxy(*cubeVB, "PointLightMesh");
    pointLightMesh->SetMaterial(lightMaterial.GetMaterialBase());
    pointLight->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(0, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.3f));
    pointLight->AddComponent<Snowglobe::Engine::MeshComponent>(pointLightMesh);
    pointLight->AddComponent<Snowglobe::RenderOpenGL::PointLightComponent>();

    auto pointLight2 = manager->CreateEntity(Snowglobe::Tags::Lights());
    pointLight2->SetName("PointLight2" );
    auto pointLight2Mesh = _renderSystem->CreateMeshProxy(*cubeVB, "PointLight2Mesh");
    pointLight2Mesh->SetMaterial(lightMaterial.GetMaterialBase());
    pointLight2->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(-1, -3.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.3f));
    pointLight2->AddComponent<Snowglobe::Engine::MeshComponent>(pointLight2Mesh);
    Snowglobe::RenderOpenGL::PointLight p;
    p.LightColor = glm::vec3(1, .0f, .0f);
    pointLight2->AddComponent<Snowglobe::RenderOpenGL::PointLightComponent>(p);
    
    auto spotLight = manager->CreateEntity(Snowglobe::Tags::Lights());
    spotLight->SetName("SpotLight" );
    auto spotLightMesh = _renderSystem->CreateMeshProxy(*cubeVB, "SpotLightMesh");
    spotLightMesh->SetMaterial(lightMaterial.GetMaterialBase());
    spotLight->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(2, 2.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.3f));
    spotLight->AddComponent<Snowglobe::Engine::MeshComponent>(spotLightMesh);
    spotLight->AddComponent<Snowglobe::RenderOpenGL::SpotLightComponent>();

    auto directionalLight = manager->CreateEntity(Snowglobe::Tags::Lights());
    directionalLight->SetName("DirectionalLight" );
    directionalLight->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(2, 2.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.3f));
    directionalLight->AddComponent<Snowglobe::RenderOpenGL::DirectionalLightComponent>();
    
    std::vector<SceneCube> cubes = {
        {glm::vec3(3, 1, -1), glm::vec3(14, 0, 25), glm::vec3(1.0f), glm::vec4(1.0f)},
        {glm::vec3(-2, 0, -6), glm::vec3(72, 0, 34), glm::vec3(1.5f), glm::vec4(1.0f)},
        {glm::vec3(0, -1, -4), glm::vec3(25, 0, 52), glm::vec3(0.5f), glm::vec4(1.0f)},
        {glm::vec3(-2, -2, -2), glm::vec3(50, 0, 39), glm::vec3(1.0f), glm::vec4(1.0f)},
        {glm::vec3(3, -3, -5), glm::vec3(87, 0, 23), glm::vec3(1.0f), glm::vec4(1.0f)},
        {glm::vec3(0, -5, 0), glm::vec3(0), glm::vec3(10.0f, 0.5f, 10.0f ), glm::vec4(1.0f)},
    };
    
    for (int i = 0; i < cubes.size(); ++i)
    {
        auto& cubeDesc = cubes[i];
        auto cube = manager->CreateEntity();
        cube->SetName("Cube" + std::to_string(i));
        auto mesh = _renderSystem->CreateMeshProxy(*cubeNormalVB, "CubeMesh");
        mesh->SetMaterial(containerMaterial.GetMaterialBase());
        
        cube->AddComponent<Snowglobe::Core::TransformComponent>(cubeDesc.Position, cubeDesc.Rotation, cubeDesc.Scale);
        cube->AddComponent<Snowglobe::Engine::MeshComponent>(mesh);
        _cubes.push_back(cube);
    }

    auto sphere = manager->CreateEntity();
    sphere->SetName("Sphere");
    auto sphereMesh = _renderSystem->CreateMeshProxy(*sphereNormalVB, "SphereMesh");
    sphereMesh->SetMaterial(containerMaterial.GetMaterialBase());
    sphere->AddComponent<Snowglobe::Core::TransformComponent>(glm::vec3(0, -1, 0), glm::vec3(0.0f), glm::vec3(0.5f));
    sphere->AddComponent<Snowglobe::Engine::MeshComponent>(sphereMesh);
    
    _renderSystem->GetCamera().SetMode(Snowglobe::Render::CameraMode::Perspective);
    _isOrthographic = false;
}

void LightTests::Run()
{
    _spectator.Update();
}


void LightTests::AddQuad(std::vector<Snowglobe::Render::PositionUVVertex>& vertices, glm::mat4x4 transform)
{
    const Snowglobe::Render::PositionUVVertex quad[6] = {
        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)},
    };

    for (int i = 0; i < 6; ++i)
    {
        vertices.push_back({transform * glm::vec4(quad[i].position, 1.0f), quad[i].uv});
    }
}

void LightTests::AddQuad(std::vector<Snowglobe::Render::PositionNormalUVVertex>& vertices, glm::mat4x4 transform)
{
    const Snowglobe::Render::PositionUVVertex quad[6] = {
        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)},
    };

    for (int i = 0; i < 6; ++i)
    {
        vertices.push_back({transform * glm::vec4(quad[i].position, 1.0f), transform * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), quad[i].uv});
    }
}

void LightTests::AddSphere(std::vector<Snowglobe::Render::PositionNormalUVVertex>& vertices, glm::mat4x4 transform)
{
    const std::vector<Snowglobe::Render::PositionNormalUVVertex> octahedron = {
        {{0, 1, 0}, {0, 1, 0}, {0.5f, 0.5f}}, {{1, 0, 0}, {1, 0, 0}, {0, 1} }, {{0, 0, 1}, {0, 0, 1}, {1, 1}},
        {{0, 1, 0}, {0, 1, 0}, {0.5f, 0.5f}}, {{0, 0, 1}, {0, 0, 1}, {1, 1}}, {{-1, 0, 0}, {-1, 0, 0}, {1, 0}},
        {{0, 1, 0}, {0, 1, 0}, {0.5f, 0.5f}}, {{-1, 0, 0}, {-1, 0, 0}, {1, 0}}, {{0, 0, -1}, {0, 0, -1}, {0, 0}},
        {{0, 1, 0}, {0, 1, 0}, {0.5f, 0.5f}}, {{0, 0, -1}, {0, 0, -1}, {0, 0}}, {{1, 0, 0}, {1, 0, 0}, {0, 1}},
        {{0, -1, 0}, {0, -1, 0}, {0.5f, 0.5f}}, {{1, 0, 0}, {1, 0, 0}, {0, 1}}, {{0, 0, 1}, {0, 0, 1}, {1, 1}},
        {{0, -1, 0}, {0, -1, 0}, {0.5f, 0.5f}}, {{0, 0, 1}, {0, 0, 1}, {1, 1}}, {{-1, 0, 0}, {-1, 0, 0}, {1, 0}},
        {{0, -1, 0}, {0, -1, 0}, {0.5f, 0.5f}}, {{-1, 0, 0}, {-1, 0, 0}, {1, 0}}, {{0, 0, -1}, {0, 0, -1}, {0, 0}},
        {{0, -1, 0}, {0, -1, 0}, {0.5f, 0.5f}}, {{0, 0, -1}, {0, 0, -1}, {0, 0}}, {{1, 0, 0}, {1, 0, 0}, {0, 1}},
    };
    vertices.insert(vertices.end(), octahedron.begin(), octahedron.end());

    const int subdevision = 4;
    for (int i = 0; i < subdevision; ++i)
    {
        std::vector<Snowglobe::Render::PositionNormalUVVertex> newVertices;
        for (int j = 0; j < vertices.size(); j += 3)
        {
            auto v0 = vertices[j];
            auto v1 = vertices[j + 1];
            auto v2 = vertices[j + 2];

            glm::vec3 v01 = glm::normalize((v0.position + v1.position) / 2.0f);
            glm::vec3 v12 = glm::normalize((v1.position + v2.position) / 2.0f);
            glm::vec3 v20 = glm::normalize((v2.position + v0.position) / 2.0f);

            glm::vec2 uv01 = (v0.uv + v1.uv) / 2.0f;
            glm::vec2 uv12 = (v1.uv + v2.uv) / 2.0f;
            glm::vec2 uv20 = (v2.uv + v0.uv) / 2.0f;

            newVertices.push_back({v0.position, v0.position, v0.uv});
            newVertices.push_back({v01, v01, uv01});
            newVertices.push_back({v20, v20, uv20});

            newVertices.push_back({v1.position, v1.position, v1.uv});
            newVertices.push_back({v12, v12, uv12});
            newVertices.push_back({v01, v01, uv01});

            newVertices.push_back({v2.position, v2.position, v2.uv});
            newVertices.push_back({v20, v20, uv20});
            newVertices.push_back({v12, v12, uv12});

            newVertices.push_back({v01, v01, uv01});
            newVertices.push_back({v12, v12, uv12});
            newVertices.push_back({v20, v20, uv20});
        }

        vertices = newVertices;
    }
}
