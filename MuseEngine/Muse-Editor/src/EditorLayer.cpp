#include "EditorLayer.h"
#include "Core/Application.h"
#include "ViewPort.h"
#include "Editor.h"
#include "FileBrowser.h"
#include "Camera/CameraJob.h"
#include <Camera\CameraComponent.h>
#include <Camera\OrthographicEditorCameraControllerComponent.h>

namespace Muse
{
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

    EditorLayer::EditorLayer()
    {

    }

    void EditorLayer::OnAttach()
    {
        MUSE_PROFILE_FUNCTION();

        FrameBufferProperties frameBufferSpecification;

        frameBufferSpecification.Width = Application::Get().GetWindow()->GetWidth();
        frameBufferSpecification.Height = Application::Get().GetWindow()->GetHeight();
        m_Framebuffer = FrameBuffer::Create(frameBufferSpecification);


        auto entity = Entity::Create();

        ComponentManager<TransformComponent>::Add(entity, {});
        ComponentManager<CameraComponent>::Add(entity, {});
        ComponentManager<OrthographicEditorCameraControllerComponent>::Add(entity, {});

        Muse::RenderCommand::Init();
        Muse::Renderer2D::Init();

        std::shared_ptr<Muse::SceneOld> scene = Muse::SceneOld::Create();
        Muse::ResourceManager::Add("Game2DTestScene", scene);
        Muse::SceneManagerOld::SwitchScene(scene);

        Muse::CameraComponentOld* cameraComponent = Muse::CameraComponentOld::GetMain();
        cameraComponent->SetZoomLevel(5);

        m_SpriteSheet = Muse::ResourceManager::Load<Muse::Texture2D>("assets/topdown/kenneyrpgpack/Spritesheet/RPGpack_sheet_2X.png");

        m_TreeTexture = Muse::SubTexture2D::Create(m_SpriteSheet, { 0, 1 }, { 128.0f, 128.0f }, { 1, 2 });

        m_TextureMap['D'] = Muse::SubTexture2D::Create(m_SpriteSheet, { 6, 11 }, { 128.0f, 128.0f });
        m_TextureMap['W'] = Muse::SubTexture2D::Create(m_SpriteSheet, { 11, 11 }, { 128.0f, 128.0f });

        m_MapWidth = s_MapWidth;
        m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

        Application::Get().GetJobManager()->Add<CameraJob>(Muse::JobType::Gameplay);
    }

    void EditorLayer::OnDetach()
    {

    }

    void EditorLayer::OnUpdate(float deltaTime)
    {
        //Resize Framebuffer:
        if (FrameBufferProperties spec = m_Framebuffer->GetProperties();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        //Rendering:
        m_Framebuffer->Bind();



        Muse::Renderer2D::BeginScene(Muse::CameraComponentOld::GetMain()->GetViewMatrix());

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

        Muse::Renderer2D::EndScene();

        m_Framebuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender()
    {
        Editor::StartDockSpace();

        FileBrowser::Render();

        ViewPort::Render(m_Framebuffer->GetColorAttachmentRendererID(), m_ViewportSize, m_AspectRatio);

        if (CameraComponentOld::GetMain() != nullptr)
        {
            CameraComponentOld::GetMain()->SetProjection(m_AspectRatio, CameraComponentOld::GetMain()->GetZoomLevel());
        }

        Editor::EndDockSpace();
    }
}