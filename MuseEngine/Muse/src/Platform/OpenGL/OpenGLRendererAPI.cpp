#include "MusePCH.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Muse
{
    void OpenGLRendererAPI::SetClearColor(const glm::vec4& a_Color)
    {
        glClearColor(a_Color.r, a_Color.g, a_Color.b, a_Color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& a_VertexArray)
    {
        glDrawElements(GL_TRIANGLES, a_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}
