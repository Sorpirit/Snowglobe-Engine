#include "CommonTests.hpp"

#include <iostream>

#include <RenderSystem.hpp>

#include <BasicShapeFactory.hpp>

#include "CommonVertexLayouts.hpp"
#include "ComponentEditor.hpp"
#include "MeshComponent.hpp"
#include "MaterialsData/TextureColorMaterialData.hpp"

void BaseShapeFactoryTests::Init()
{
    _triangleMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Triangle, glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(1.0f));
    _planeMesh =  _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    _diskMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Disk, glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(1.0f));

    auto yellowMaterial = _renderSystem->CreateMaterialInstance<Snowglobe::Render::BasicShapeMaterial>();
    yellowMaterial.Properties()->color = glm::vec3(1.0f, 1.0f, 0.0f);

    _triangleMesh->SetMaterial(yellowMaterial.GetMaterialBase());
    _planeMesh->SetMaterial(yellowMaterial.GetMaterialBase());
    _diskMesh->SetMaterial(_gradientMaterial.GetMaterialBase());

}

void BaseShapeFactoryTests::Run()
{
    float time = Snowglobe::SnowCore::EngineTime::GetTimeSinceStart();

    float x = (sin(time) + 1.0f) * 0.5f;
    
    _triangleMesh->SetRotation(glm::vec3(0.0f, 0.0f, x * 360));
    _triangleMesh->SetScale(glm::vec3(
        std::lerp(_scale1.x, _scale2.x, x),
        std::lerp(_scale1.y, _scale2.y, x),
        1.0f));
    
    _planeMesh->SetPosition(glm::vec3(cos(x * 2.0f * 3.14159f), sin(x * 2.0f * 3.14159f), 0.0f));
    _planeMesh->SetRotation(glm::vec3(0.0f, 0.0f, x * 360));

    _gradientMaterial.Properties()->color = glm::vec3(
        std::lerp(_gradient1.x, _gradient2.x, x),
        std::lerp(_gradient1.y, _gradient2.y, x),
        std::lerp(_gradient1.z, _gradient2.z, x));

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
    auto tileTexture = _fileSystem.LoadTexture(_fileSystem.CrateFileHandle("Textures/wall.jpg"));
    auto containerTexture = _fileSystem.LoadTexture(_fileSystem.CrateFileHandle("Textures/container.jpg"));
    auto awesomeTexture = _fileSystem.LoadTexture(_fileSystem.CrateFileHandle("Textures/awesomeface.png"));
    
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
    materialL.Properties()->color = glm::vec3(1.0f, 0.0f, 0.0f);
    auto materialR = _renderSystem->CreateMaterialInstance<Snowglobe::Render::BasicShapeMaterial>();
    materialR.Properties()->color = glm::vec3(0.0f, 0.0f, 1.0f);
    auto materialG = _renderSystem->CreateMaterialInstance<Snowglobe::Render::BasicShapeMaterial>();
    materialG.Properties()->color = glm::vec3(0.1f, 1.0f, 0.1f);

    auto manager = _engine.GetEntityManager();

    auto ballL = manager->CreateEntity();
    auto ballLMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Disk, glm::vec3(0.0f), glm::vec3(1.0f));
    ballL->AddComponent<Snowglobe::SnowEngine::DebugComponent>();
    ballL->AddComponent<Snowglobe::SnowCore::TransformComponent>(glm::vec3(-3.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    ballL->AddComponent<Snowglobe::SnowEngine::Physics2DComponent>(glm::vec2(1.0f, 0.2f), 0.0f, 1.0f, 0.0f, 1.0f);
    ballL->AddComponent<Snowglobe::SnowEngine::Collider2DComponent>(Snowglobe::SnowEngine::CollisionShapeType::Circle);
    ballL->AddComponent<Snowglobe::SnowEngine::MeshComponent>(ballLMesh);
    ballL->AddComponent<Snowglobe::SnowEngine::BaseComponentMaterial>(&materialL.GetMaterialBase());
    ballLMesh->SetMaterial(materialL.GetMaterialBase());

    auto ballR = manager->CreateEntity();
    auto ballRMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Disk, glm::vec3(0.0f), glm::vec3(1.0f));
    ballR->AddComponent<Snowglobe::SnowEngine::DebugComponent>();
    ballR->AddComponent<Snowglobe::SnowCore::TransformComponent>(glm::vec3(4.0f, 0.5f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    ballR->AddComponent<Snowglobe::SnowEngine::Physics2DComponent>(glm::vec2(-1.0f, -0.2f), 0.0f, 1.0f, 0.0f, 1.0f);
    ballR->AddComponent<Snowglobe::SnowEngine::Collider2DComponent>(Snowglobe::SnowEngine::CollisionShapeType::Circle);
    ballR->AddComponent<Snowglobe::SnowEngine::MeshComponent>(ballRMesh);
    ballR->AddComponent<Snowglobe::SnowEngine::BaseComponentMaterial>(&materialR.GetMaterialBase());
    ballRMesh->SetMaterial(materialR.GetMaterialBase());

    auto planeL = manager->CreateEntity();
    auto planeLMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    planeL->AddComponent<Snowglobe::SnowEngine::DebugComponent>();
    planeL->AddComponent<Snowglobe::SnowCore::TransformComponent>(glm::vec3(-4.0f, -0.5f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    planeL->AddComponent<Snowglobe::SnowEngine::Physics2DComponent>(glm::vec2(-2.0f, 0.0f), 0.0f, 1.0f, 0.0f, 1.0f);
    planeL->AddComponent<Snowglobe::SnowEngine::Collider2DComponent>(Snowglobe::SnowEngine::CollisionShapeType::AABB);
    planeL->AddComponent<Snowglobe::SnowEngine::MeshComponent>(planeLMesh);
    planeL->AddComponent<Snowglobe::SnowEngine::BaseComponentMaterial>(&materialL.GetMaterialBase());
    planeLMesh->SetMaterial(materialL.GetMaterialBase());

    auto planeR = manager->CreateEntity();
    auto planeRMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    planeR->AddComponent<Snowglobe::SnowEngine::DebugComponent>();
    planeR->AddComponent<Snowglobe::SnowCore::TransformComponent>(glm::vec3(3.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    planeR->AddComponent<Snowglobe::SnowEngine::Physics2DComponent>(glm::vec2(-2.0f, 0.0f), 0.0f, 1.0f, 0.0f, 1.0f);
    planeR->AddComponent<Snowglobe::SnowEngine::Collider2DComponent>(Snowglobe::SnowEngine::CollisionShapeType::AABB);
    planeR->AddComponent<Snowglobe::SnowEngine::MeshComponent>(planeRMesh);
    planeR->AddComponent<Snowglobe::SnowEngine::BaseComponentMaterial>(&materialR.GetMaterialBase());
    planeRMesh->SetMaterial(materialR.GetMaterialBase());

    auto groundWall = manager->CreateEntity();
    auto groundWallMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    groundWall->AddComponent<Snowglobe::SnowEngine::DebugComponent>();
    groundWall->AddComponent<Snowglobe::SnowCore::TransformComponent>(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(4.0f, 0.5f, 1.0f));
    groundWall->AddComponent<Snowglobe::SnowEngine::Collider2DComponent>(Snowglobe::SnowEngine::CollisionShapeType::AABB);
    groundWall->AddComponent<Snowglobe::SnowEngine::MeshComponent>(groundWallMesh);
    groundWall->AddComponent<Snowglobe::SnowEngine::BaseComponentMaterial>(&materialG.GetMaterialBase());
    groundWallMesh->SetMaterial(materialG.GetMaterialBase());

    auto wallBottom = manager->CreateEntity();
    auto wallBottomMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    wallBottom->SetName("wallBottom");
    wallBottom->AddComponent<Snowglobe::SnowEngine::DebugComponent>();
    wallBottom->AddComponent<Snowglobe::SnowCore::TransformComponent>(glm::vec3(0.0f, -2.5f, 0.0f), glm::vec3(0.0f), glm::vec3(8.5f, 0.5f, 1.0f));
    wallBottom->AddComponent<Snowglobe::SnowEngine::Collider2DComponent>(Snowglobe::SnowEngine::CollisionShapeType::AABB);
    wallBottom->AddComponent<Snowglobe::SnowEngine::MeshComponent>(wallBottomMesh);
    wallBottomMesh->SetMaterial(materialG.GetMaterialBase());

    auto wallTop = manager->CreateEntity();
    auto wallTopMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    wallTop->SetName("wallTop");
    wallTop->AddComponent<Snowglobe::SnowEngine::DebugComponent>();
    wallTop->AddComponent<Snowglobe::SnowCore::TransformComponent>(glm::vec3(0.0f, 2.5f, 0.0f), glm::vec3(0.0f), glm::vec3(8.5f, 0.5f, 1.0f));
    wallTop->AddComponent<Snowglobe::SnowEngine::Collider2DComponent>(Snowglobe::SnowEngine::CollisionShapeType::AABB);
    wallTop->AddComponent<Snowglobe::SnowEngine::MeshComponent>(wallTopMesh);
    wallTopMesh->SetMaterial(materialG.GetMaterialBase());
    
    auto wallRight = manager->CreateEntity();
    auto wallRightMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    wallRight->SetName("wallRight");
    wallRight->AddComponent<Snowglobe::SnowEngine::DebugComponent>();
    wallRight->AddComponent<Snowglobe::SnowCore::TransformComponent>(glm::vec3(4.5f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f, 5.0f, 1.0f));
    wallRight->AddComponent<Snowglobe::SnowEngine::Collider2DComponent>(Snowglobe::SnowEngine::CollisionShapeType::AABB);
    wallRight->AddComponent<Snowglobe::SnowEngine::MeshComponent>(wallRightMesh);
    wallRightMesh->SetMaterial(materialG.GetMaterialBase());

    auto wallLeft = manager->CreateEntity();
    auto wallLeftMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    wallLeft->SetName("wallLeft");
    wallLeft->AddComponent<Snowglobe::SnowEngine::DebugComponent>();
    wallLeft->AddComponent<Snowglobe::SnowCore::TransformComponent>(glm::vec3(-4.5f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f, 5.0f, 1.0f));
    wallLeft->AddComponent<Snowglobe::SnowEngine::Collider2DComponent>(Snowglobe::SnowEngine::CollisionShapeType::AABB);
    wallLeft->AddComponent<Snowglobe::SnowEngine::MeshComponent>(wallLeftMesh);
    wallLeftMesh->SetMaterial(materialG.GetMaterialBase());
}

void Phyiscs2DTests::Run()
{
}

void CameraTests::Init()
{
    auto containerTexture = _fileSystem.LoadTexture(_fileSystem.CrateFileHandle("Textures/container.jpg"));
    
    auto texture = _renderSystem->CreateTexture2D(*containerTexture, {
        Snowglobe::Render::TextureWrap::Repeat,
        Snowglobe::Render::TextureWrap::Repeat,
        Snowglobe::Render::TextureFilter::Linear,
        Snowglobe::Render::TextureFilter::Linear});

    
    std::vector<Snowglobe::Render::PositionUVVertex> vertices;
    AddQuad(vertices, glm::mat4x4(1.0f));
    AddQuad(vertices, glm::rotate(glm::mat4x4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    AddQuad(vertices, glm::rotate(glm::mat4x4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    AddQuad(vertices, glm::rotate(glm::mat4x4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    AddQuad(vertices, glm::rotate(glm::mat4x4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    AddQuad(vertices, glm::rotate(glm::mat4x4(1.0f), glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    
    auto vertexBuffer = _renderSystem->AllocateVertexBufferPtr(vertices);
    
    auto textureMaterial = _renderSystem->CreateMaterialInstance<Snowglobe::Render::MaterialsData::TextureColorMaterialData>();
    textureMaterial.Properties()->texture = texture;
    textureMaterial.Properties()->color = glm::vec4(1.0f);

    for (int i = 0; i < _cubes.size(); ++i)
    {
        auto mesh = _renderSystem->CreateMeshProxy(*vertexBuffer, "CubeMesh");
        mesh->SetMaterial(textureMaterial.GetMaterialBase());
        _cubes[i]->AddComponent<Snowglobe::SnowCore::TransformComponent>();
        _cubes[i]->AddComponent<Snowglobe::SnowEngine::MeshComponent>(mesh);
        
    }

    Snowglobe::SnowCore::TransformComponent* transform = nullptr;
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
    
    if (_window->GetInput().IsKeyPressed(Snowglobe::SnowCore::Key::T))
    {
        _isOrthographic = !_isOrthographic;
        _renderSystem->GetCamera().SetMode(_isOrthographic ? Snowglobe::Render::CameraMode::Orthographic : Snowglobe::Render::CameraMode::Perspective);
    }

    // float timeDelta = Snowglobe::SnowCore::EngineTime::GetDeltaEngineFrameTime();
    // for (int i = 0; i < _cubes.size(); ++i)
    // {
    //     auto cubeRotation = _cubes[i]->GetRotation();
    //     cubeRotation.y += 45.0f * timeDelta;
    //     _cubes[i]->SetRotation(cubeRotation);
    // }
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
