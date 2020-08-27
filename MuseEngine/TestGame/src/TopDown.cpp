#include "TopDown.h"

#include "imgui/imgui.h"

#include "Muse.h"
#include "Mode.h"

#include "Core/ECS/Job/Job.h"
#include "Core/ECS/Job/JobManager.h"
#include "Core/ECS/Job/Render2DJob.h"
#include "Core/ECS/Component/Render2DComponent.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/Gameplay/Component/CameraComponent.cpp"
#include "Core/Renderer/SubTexture2D.h"
#include "Core/ECS/Entity/EntityDebugger.h"

static uint32_t s_MapWidth = 24;
static const char* s_MapTiles =
"WWWWWWWWDDDDWWWWWWWWWWWW"
"WWWWWDDDWWWWDDDWWWWWWWWW"
"WWWWDDWWWWWWWWDDWWWWWWWW"
"WWWDDWWWWWWWWWDDDWWWWWWW"
"WWWWWDDWWWWWWWWWDWWWWWWW"
"WWWWWWDDWWWWWWWWDWWWWWWW"
"WWWWWWWWDDDDDDDWWWWWWWWW"
"WWWWWWWWWWDDDWWWWWWWWWWW";


#if GAME_TOPDOWN
#include "EntryPoint.h"
Muse::Application* Muse::CreateApplication()
{
    return new TopDown();
}
#endif

void TopDown::OnStart()
{
    Muse::RenderCommand::Init();
    Muse::Renderer2D::Init();

    std::shared_ptr<Muse::SceneOld> scene = Muse::SceneOld::Create();
    Muse::ResourceManager::Add("Game2DTestScene", scene);
    Muse::SceneManagerOld::SwitchScene(scene);

    Muse::CameraComponent* cameraComponent = Muse::CameraComponent::GetMain();
    cameraComponent->SetZoomLevel(5);

    m_SpriteSheet = Muse::ResourceManager::Load<Muse::Texture2D>("assets/topdown/kenneyrpgpack/Spritesheet/RPGpack_sheet_2X.png");

    m_TreeTexture = Muse::SubTexture2D::Create(m_SpriteSheet, { 0, 1 }, { 128.0f, 128.0f }, { 1, 2 });

    m_TextureMap['D'] = Muse::SubTexture2D::Create(m_SpriteSheet, { 6, 11 }, { 128.0f, 128.0f });
    m_TextureMap['W'] = Muse::SubTexture2D::Create(m_SpriteSheet, { 11, 11 }, { 128.0f, 128.0f });

    m_MapWidth = s_MapWidth;
    m_MapHeight = strlen(s_MapTiles) / s_MapWidth;
}

void TopDown::OnRender()
{
    Muse::Renderer2D::BeginScene(*Muse::CameraComponent::GetMain());


    Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Muse::RenderCommand::Clear();

    for (uint32_t y = 0; y < m_MapHeight; y++)
    {
        for (uint32_t x = 0; x < m_MapWidth; x++)
        {
            char tileType = s_MapTiles[x + y * m_MapWidth];

            if (m_TextureMap.find(tileType) == m_TextureMap.end())
            {
                continue;
            }

            Muse::Renderer2D::DrawQuad({ x, y, 0 }, { 1, 1 }, 0, m_TextureMap[tileType]);
        }
    }

    Muse::Renderer2D::DrawQuad({ 0, 0, 0 }, { 1, 2 }, 0, m_TreeTexture);

    Muse::Renderer2D::ResetStatistics();

    Muse::Renderer2D::EndScene();

}

void TopDown::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::Spacing();

    auto stats = Muse::Renderer2D::GetStatistics();

    ImGui::Text("Renderer2D Statistics:");
    ImGui::Text("FPS: %f", 1 / GetDeltaTime());
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quad Count: %d", stats.QuadCount);
    ImGui::Text("Vertex Count: %d", stats.GetTotalVertex());
    ImGui::Text("Index Count: %d", stats.GetTotalIndex());

    ImGui::End();
}