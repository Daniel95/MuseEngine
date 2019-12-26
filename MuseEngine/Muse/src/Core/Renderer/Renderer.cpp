#include "MusePCH.h"
#include "Renderer.h"
#include "RenderCommand.h"

namespace Muse
{
    void Renderer::BeginScene()
    {
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<VertexArray>& a_VertexArray)
    {
        a_VertexArray->Bind();
        RenderCommand::DrawIndexed(a_VertexArray);
    }
}
