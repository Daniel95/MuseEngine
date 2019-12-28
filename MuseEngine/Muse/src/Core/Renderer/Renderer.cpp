﻿#include "MusePCH.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Core/Gameplay/Component/CameraComponent.h"

namespace Muse
{
    Renderer::SceneData* Renderer::s_SceneData = new SceneData();

    void Renderer::BeginScene(const CameraComponent& a_Camera)
    {
        s_SceneData->ViewProjectionMatrix = a_Camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& a_Shader, const std::shared_ptr<VertexArray>& a_VertexArray)
    {
        a_Shader->Bind();
        a_Shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);

        a_VertexArray->Bind();
        RenderCommand::DrawIndexed(a_VertexArray);
    }
}
