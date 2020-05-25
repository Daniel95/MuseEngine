#include "MusePCH.h"
#include "RenderCommand.h"

#include "VertexArray.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Core/Instrumentor.h"

namespace Muse
{
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

    void RenderCommand::Init()
    {
        MUSE_PROFILE_FUNCTION();

        s_RendererAPI->Init();
    }

    void RenderCommand::SetViewport(uint32_t a_X, uint32_t a_Y, uint32_t a_Width, uint32_t a_Height)
    {
        MUSE_PROFILE_FUNCTION();

        s_RendererAPI->SetViewport(a_X, a_Y, a_Width, a_Height);
    }

    void RenderCommand::SetClearColor(const glm::vec4& a_Color)
    {
        MUSE_PROFILE_FUNCTION();

        s_RendererAPI->SetClearColor(a_Color);
    }

    void RenderCommand::Clear()
    {
        MUSE_PROFILE_FUNCTION();

        s_RendererAPI->Clear();
    }

    void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& a_VertexArray, uint32_t a_IndexCount)
    {
        MUSE_PROFILE_FUNCTION();

        s_RendererAPI->DrawIndexed(a_VertexArray, a_IndexCount);
    }
}
