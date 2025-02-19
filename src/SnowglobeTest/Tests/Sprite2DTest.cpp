#include "Sprite2DTest.hpp"

#include "SpriteRenderComponent.hpp"
#include "TransformComponent.hpp"

namespace Snowglobe {

void Sprite2DTest::Init()
{
    RuntimeTest::Init();

    auto manager = _engine->GetEntityManager();
    auto entity = manager->CreateEntity();
    entity->AddComponent<Core::TransformComponent>();
    entity->AddComponent<Render::SpriteRenderComponent>();

    Render::SpriteRenderComponent* spritePtr = nullptr;
    if (entity->QueryComponent(spritePtr))
    {
        spritePtr->File = Core::SnowFileHandle("Textures/awesomeface.png");
        auto fsprite = _fileSystem->LoadTexture(spritePtr->File);
        auto textureDesc = Snowglobe::Render::Texture2DDescriptor{
            Snowglobe::Render::TextureWrap::Repeat, Snowglobe::Render::TextureWrap::Repeat,
            Snowglobe::Render::TextureFilter::Nearest, Snowglobe::Render::TextureFilter::Nearest};
        spritePtr->Sprite = _renderSystem->CreateTexture2D(*fsprite, textureDesc);
    }
}

void Sprite2DTest::Run() {}

} // Snowglobe