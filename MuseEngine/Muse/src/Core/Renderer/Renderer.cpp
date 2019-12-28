#include "MusePCH.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Muse
{
    Renderer::SceneData* Renderer::m_SceneData = new SceneData();

    void Renderer::BeginScene(const OrthographicCamera& a_Camera)
    {
        m_SceneData->ViewProjectionMatrix = a_Camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& a_Shader, const std::shared_ptr<VertexArray>& a_VertexArray)
    {
        a_Shader->Bind();
        a_Shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);

        a_VertexArray->Bind();
        RenderCommand::DrawIndexed(a_VertexArray);
    }
}
