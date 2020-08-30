#include "EditorLayer.h"
#include "Core/Application.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Scene/Scene.h"
#include "Editor.h"
#include "FileBrowser.h"
#include "Camera/CameraComponent.h"
#include "Camera/OrthographicEditorCameraControllerComponent.h"
#include "Camera/OrthographicEditorCameraControllerJob.h"
#include "Core/ECS/Entity/Entity.h"
#include "Core/ECS/Component/Render2DComponent.h"

#include "imgui.h"
#include "Core/ECS/Job/Render2DJob.h"

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
            m_CameraEntity = Entity::Create();

            CameraComponent cameraComponent = {};
            cameraComponent.SetZoomLevel(5);

            m_CameraEntity.AddComponent<CameraComponent>(cameraComponent);
            m_CameraEntity.AddComponent<OrthographicEditorCameraControllerComponent>();
        }

        //Make Level
        {
            TransformComponent transformComponent;
            transformComponent.SetLocalPosition({ 0, 2, 0 });
            auto entity = Entity::Create(transformComponent);

            Render2DComponent& render2DComponent = entity.AddComponent<Render2DComponent>();
            render2DComponent.texture = m_TreeTexture->GetTexture();
        }

        Muse::RenderCommand::Init();
        Muse::Renderer2D::Init();

        Application::Get().GetJobManager()->Add<Render2DJob>(Muse::JobType::Renderer);
        Application::Get().GetJobManager()->Add<OrthographicEditorCameraControllerJob>(Muse::JobType::Gameplay);
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