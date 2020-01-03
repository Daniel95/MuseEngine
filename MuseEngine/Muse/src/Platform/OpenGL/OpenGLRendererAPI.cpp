#include "MusePCH.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Muse
{
    void OpenGLRendererAPI::Init()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void OpenGLRendererAPI::SetViewport(uint32_t a_X, uint32_t a_Y, uint32_t a_Width, uint32_t a_Height)
    {
        glViewport(a_X, a_Y, a_Width, a_Height);
    }

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
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}