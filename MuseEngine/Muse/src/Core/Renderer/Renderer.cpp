#include "MusePCH.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Core/Gameplay/Component/CameraComponent.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Muse
{
    Renderer::SceneData* Renderer::s_SceneData = new SceneData();

    void Renderer::Init()
    {
        RenderCommand::Init();
    }

    void Renderer::OnWindowResize(uint32_t a_Width, uint32_t a_Height)
    {
        RenderCommand::SetViewport(0, 0, a_Width, a_Height);
    }

    void Renderer::BeginScene(const CameraComponent& a_Camera)
    {
        s_SceneData->ViewProjectionMatrix = a_Camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& a_Shader, const std::shared_ptr<VertexArray>& a_VertexArray, const glm::mat4& a_Transform)
    {
        a_Shader->Bind();

        std::dynamic_pointer_cast<OpenGLShader>(a_Shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(a_Shader)->UploadUniformMat4("u_Transform", a_Transform);

        a_VertexArray->Bind();
        RenderCommand::DrawIndexed(a_VertexArray);
    }
}
