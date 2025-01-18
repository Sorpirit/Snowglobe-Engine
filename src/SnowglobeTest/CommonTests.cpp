#include "CommonTests.hpp"

#include <iostream>

#include <RenderSystem.hpp>

#include <BasicShapeFactory.hpp>

#include "CommonVertexLayouts.hpp"
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
    if (_engine.QuerySystem(_physicsEngine))
    {
        std::cout << "Failed to get physics engine" << std::endl;
    }

    auto materialL = _renderSystem->CreateMaterialInstance<Snowglobe::Render::BasicShapeMaterial>();
    materialL.Properties()->color = glm::vec3(1.0f, 0.0f, 0.0f);
    auto materialR = _renderSystem->CreateMaterialInstance<Snowglobe::Render::BasicShapeMaterial>();
    materialR.Properties()->color = glm::vec3(0.0f, 0.0f, 1.0f);
    auto materialG = _renderSystem->CreateMaterialInstance<Snowglobe::Render::BasicShapeMaterial>();
    materialG.Properties()->color = glm::vec3(0.1f, 1.0f, 0.1f);


    Snowglobe::SnowEngine::SnowEntity& ballL = _engine.CreateEntity();
    auto ballLMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Disk, glm::vec3(0.0f), glm::vec3(1.0f));
    ballLMesh->SetMaterial(materialL.GetMaterialBase());
    _ballLMeshC = std::make_shared<Snowglobe::SnowEngine::MeshComponent>(*ballLMesh);
    ballL.AddComponent(_ballLMeshC.get());
    _physicsEngine->AttachCollisionComponent(ballL, Snowglobe::SnowEngine::CollisionShapeType::Circle);
    auto ballLP = _physicsEngine->AttachPhysicsComponent(ballL);

    Snowglobe::SnowEngine::SnowEntity& ballR = _engine.CreateEntity();
    auto ballRMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Disk, glm::vec3(0.0f), glm::vec3(1.0f));
    ballRMesh->SetMaterial(materialR.GetMaterialBase());
    _ballRMeshC = std::make_shared<Snowglobe::SnowEngine::MeshComponent>(*ballRMesh);
    ballR.AddComponent(_ballRMeshC.get());
    _physicsEngine->AttachCollisionComponent(ballR, Snowglobe::SnowEngine::CollisionShapeType::Circle);
    auto ballRP = _physicsEngine->AttachPhysicsComponent(ballR);

    Snowglobe::SnowEngine::SnowEntity& planeL = _engine.CreateEntity();
    auto planeLMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    planeLMesh->SetMaterial(materialL.GetMaterialBase());
    _planeLMeshC = std::make_shared<Snowglobe::SnowEngine::MeshComponent>(*planeLMesh);
    planeL.AddComponent(_planeLMeshC.get());
    _physicsEngine->AttachCollisionComponent(planeL, Snowglobe::SnowEngine::CollisionShapeType::AABB);
    auto planeLP = _physicsEngine->AttachPhysicsComponent(planeL);

    Snowglobe::SnowEngine::SnowEntity& planeR = _engine.CreateEntity();
    auto planeRMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    planeRMesh->SetMaterial(materialR.GetMaterialBase());
    _planeRMeshC = std::make_shared<Snowglobe::SnowEngine::MeshComponent>(*planeRMesh);
    planeR.AddComponent(_planeRMeshC.get());
    _physicsEngine->AttachCollisionComponent(planeR, Snowglobe::SnowEngine::CollisionShapeType::AABB);
    auto planeRP = _physicsEngine->AttachPhysicsComponent(planeR);

    Snowglobe::SnowEngine::SnowEntity& groundWall = _engine.CreateEntity();
    auto groundWallMesh = _shapeFactory.CreateShape(Snowglobe::Render::BasicShape::Plane, glm::vec3(0.0f), glm::vec3(1.0f));
    groundWallMesh->SetMaterial(materialG.GetMaterialBase());
    _grounWallMeshC = std::make_shared<Snowglobe::SnowEngine::MeshComponent>(*groundWallMesh);
    groundWall.AddComponent(_grounWallMeshC.get());
    _physicsEngine->AttachCollisionComponent(groundWall, Snowglobe::SnowEngine::CollisionShapeType::AABB);

    ballL.SetPosition(glm::vec3(-3.0f, 1.0f, 0.0f));
    planeL.SetPosition(glm::vec3(-4.0f, -0.5f, 0.0f));
    ballR.SetPosition(glm::vec3(4.0f, 0.5f, 0.0f));
    planeR.SetPosition(glm::vec3(3.0f,  -1.0f, 0.0f));
    
    groundWall.SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));
    groundWall.SetScale(glm::vec3(4.0f, 0.5f, 1.0f));

    ballLP->SetVelocity(glm::vec2(1.0f, 0.2f));
    planeLP->SetVelocity(glm::vec2(2.0f, 0.0f));
    ballRP->SetVelocity(glm::vec2(-1.0f, -0.2f));
    planeRP->SetVelocity(glm::vec2(-2.0f, 0.0f));
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
        _cubeMeshC.push_back(std::make_shared<Snowglobe::SnowEngine::MeshComponent>(*mesh));
        _cubes[i]->AddComponent(_cubeMeshC[i].get());
    }

    _cubes[0]->SetPosition(glm::vec3(3, 1, -1));
    _cubes[1]->SetPosition(glm::vec3(-2, 0, -6));
    _cubes[2]->SetPosition(glm::vec3(0, -1, -4));
    _cubes[3]->SetPosition(glm::vec3(-2, -2, -2));
    _cubes[4]->SetPosition(glm::vec3(3, -3, -5));

    _cubes[0]->SetRotation(glm::vec3(14, 0, 25));
    _cubes[1]->SetRotation(glm::vec3(72, 0, 34));
    _cubes[2]->SetRotation(glm::vec3(25, 0, 52));
    _cubes[3]->SetRotation(glm::vec3(50, 0, 39));
    _cubes[4]->SetRotation(glm::vec3(87, 0, 23));
    
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

    float timeDelta = Snowglobe::SnowCore::EngineTime::GetDeltaEngineFrameTime();
    for (int i = 0; i < _cubes.size(); ++i)
    {
        auto cubeRotation = _cubes[i]->GetRotation();
        cubeRotation.y += 45.0f * timeDelta;
        _cubes[i]->SetRotation(cubeRotation);
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
