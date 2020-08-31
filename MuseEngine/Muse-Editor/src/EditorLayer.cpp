#include "EditorLayer.h"
#include "Core/Application.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Scene/Scene.h"
#include "Editor.h"
#include "FileBrowser.h"
#include "Core/ECS/Component/CameraComponent.h"
#include "Core/ECS/Component/OrthographicEditorCameraControllerComponent.h"
#include "Core/ECS/Job/OrthographicEditorCameraControllerJob.h"
#include "Core/ECS/Entity/Entity.h"
#include "Core/ECS/Component/Render2DComponent.h"

#include "imgui.h"
#include "Core/ECS/Job/Render2DJob.h"
#include "Core/Input/Input.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Input/MouseButtonCodes.h"

namespace Muse
{
    void EditorLayer::OnAttach()
    {
        MUSE_PROFILE_FUNCTION();

        FrameBufferProperties frameBufferSpecification;

        frameBufferSpecification.Width = Application::Get().GetWindow()->GetWidth();
        frameBufferSpecification.Height = Application::Get().GetWindow()->GetHeight();
        m_Framebuffer = FrameBuffer::Create(frameBufferSpecification);

        std::shared_ptr<Muse::Scene> scene = Muse::Scene::Create();
        Muse::ResourceManager::Add("EditorTestScene", scene);
        Muse::SceneManager::SwitchScene(scene);

        m_SpriteSheet = Muse::ResourceManager::Load<Muse::Texture2D>("assets/topdown/kenneyrpgpack/Spritesheet/RPGpack_sheet_2X.png");
        m_TreeTexture = Muse::SubTexture2D::Create(m_SpriteSheet, { 0, 1 }, { 128.0f, 128.0f }, { 1, 2 });

        //Make Camera
        {
            m_CameraEntity = Entity::Create("Camera");

            m_CameraComponent = &m_CameraEntity.AddComponent<CameraComponent>();
            m_CameraComponent->SetZoomLevel(5);
            m_CameraEntity.AddComponent<OrthographicEditorCameraControllerComponent>();
        }

        //Make Level
        {
            TransformComponent transformComponent;
            transformComponent.SetLocalPosition({ 0, 2, 0 });
            auto entity = Entity::Create("Squad", transformComponent);

            Render2DComponent& render2DComponent = entity.AddComponent<Render2DComponent>();
            render2DComponent.texture = m_TreeTexture->GetTexture();

            class PlayerController : public ScriptableEntity
            {
            public:
                void OnCreate()
                {
                    LOG_TRACE("OnCreate");
                }

                void OnDestroy()
                {
                    LOG_TRACE("OnDestroy");
                }

                void OnUpdate(float a_DeltaTime)
                {
                    auto& transform = GetComponent<TransformComponent>();
                    float speed = 0.01f;

                    if (Input::GetKeyDown(Key::Right))
                    {
                        transform.TranslateLocal({ 1.0f * speed, 0 });
                    }
                    if (Input::GetKeyDown(Key::Left))
                    {
                        transform.TranslateLocal({ -1.0f * speed, 0 });
                    }
                    if (Input::GetKeyDown(Key::Up))
                    {
                        transform.TranslateLocal({ 0,  1.0f * speed });
                    }
                    if (Input::GetKeyDown(Key::Down))
                    {
                        transform.TranslateLocal({ 0,  -1.0f * speed });
                    }

                    LOG_TRACE("OnUpdate {0}", a_DeltaTime);
                }

            private:

            };

            //Maybe use specialized template
            entity.AddComponent<NativeScriptComponent>().Bind<PlayerController>();
        }

        Muse::RenderCommand::Init();
        Muse::Renderer2D::Init();

        Application::Get().GetJobManager()->Add<Render2DJob>(Muse::JobType::Renderer);
        Application::Get().GetJobManager()->Add<OrthographicEditorCameraControllerJob>(Muse::JobType::Gameplay);

        m_SceneHierarchyPanel.SetContext(Muse::SceneManager::GetActiveScene());
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

            Muse::SceneManager::GetActiveScene()->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
        }

        //Rendering:
        m_Framebuffer->Bind();

        const glm::mat4& projectionViewMatrix = m_CameraEntity.GetComponent<CameraComponent>().GetProjectionViewMatrix(m_CameraEntity.GetComponent<TransformComponent>());

        Muse::Renderer2D::BeginScene(projectionViewMatrix);

        Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Muse::RenderCommand::Clear();

        Muse::Renderer2D::DrawQuad({ 0, 0, 0 }, { 1, 2 }, 0, m_TreeTexture);

        Application::Get().GetJobManager()->Update(JobType::Renderer);

        Muse::Renderer2D::EndScene();

        m_Framebuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender()
    {
        Editor::StartDockSpace();

        FileBrowser::Render();

        m_SceneHierarchyPanel.OnImGuiRender();

        //ViewPort
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("Viewport");

            auto viewportSize = ImGui::GetContentRegionAvail();

            m_ViewportSize = { viewportSize.x, viewportSize.y };
            m_AspectRatio = viewportSize.x / viewportSize.y;

            m_ViewportFocused = ImGui::IsWindowFocused();
            m_ViewportHovered = ImGui::IsWindowHovered();
            Input::SetBlockInput(!m_ViewportFocused || !m_ViewportHovered);

            ImGui::Image((void*)m_Framebuffer->GetColorAttachmentRendererID(), viewportSize, { 0, 1 }, { 1, 0 });
            ImGui::End();
            ImGui::PopStyleVar();
        }

        if (CameraComponentOld::GetMain() != nullptr)
        {
            CameraComponentOld::GetMain()->SetProjection(m_AspectRatio, CameraComponentOld::GetMain()->GetZoomLevel());
        }

        Editor::EndDockSpace();
    }
}