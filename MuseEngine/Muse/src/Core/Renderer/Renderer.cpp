#include "MusePCH.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Core/Gameplay/Component/CameraComponent.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Core/Instrumentor.h"

namespace Muse
{
    Renderer::SceneData* Renderer::s_SceneData = new SceneData();

    void Renderer::Init()
    {
        MUSE_PROFILE_FUNCTION();

        RenderCommand::Init();
    }

    void Renderer::OnWindowResize(uint32_t a_Width, uint32_t a_Height)
    {
        MUSE_PROFILE_FUNCTION();

        RenderCommand::SetViewport(0, 0, a_Width, a_Height);
    }

    void Renderer::BeginScene(const CameraComponent& a_Camera)
    {
        MUSE_PROFILE_FUNCTION();

        s_SceneData->ViewProjectionMatrix = a_Camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
        MUSE_PROFILE_FUNCTION();

    }

    void Renderer::Submit(const std::shared_ptr<Shader>& a_Shader, const std::shared_ptr<VertexArray>& a_VertexArray, const glm::mat4& a_Transform)
    {
        MUSE_PROFILE_FUNCTION();

        a_Shader->Bind();

        a_Shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        a_Shader->SetMat4("u_Transform", a_Transform);

        a_VertexArray->Bind();
        RenderCommand::DrawIndexed(a_VertexArray);
    }
}
