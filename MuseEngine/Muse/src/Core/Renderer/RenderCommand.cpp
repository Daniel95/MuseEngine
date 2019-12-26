#include "MusePCH.h"
#include "RenderCommand.h"

#include "VertexArray.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Muse
{
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

    void RenderCommand::SetClearColor(const glm::vec4& a_Color)
    {
        s_RendererAPI->SetClearColor(a_Color);
    }

    void RenderCommand::Clear()
    {
        s_RendererAPI->Clear();
    }

    void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& a_VertexArray)
    {
        s_RendererAPI->DrawIndexed(a_VertexArray);
    }
}
