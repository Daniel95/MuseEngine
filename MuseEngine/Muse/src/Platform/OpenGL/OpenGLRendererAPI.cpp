#include "MusePCH.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>
#include "Core/Instrumentor.h"

namespace Muse
{
    void OpenGLRendererAPI::Init()
    {
        MUSE_PROFILE_FUNCTION();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::SetViewport(uint32_t a_X, uint32_t a_Y, uint32_t a_Width, uint32_t a_Height)
    {
        MUSE_PROFILE_FUNCTION();

        glViewport(a_X, a_Y, a_Width, a_Height);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& a_Color)
    {
        MUSE_PROFILE_FUNCTION();

        glClearColor(a_Color.r, a_Color.g, a_Color.b, a_Color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        MUSE_PROFILE_FUNCTION();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& a_VertexArray, uint32_t a_IndexCount)
    {
        MUSE_PROFILE_FUNCTION();

        uint32_t indexCount = a_IndexCount == 0 ? a_VertexArray->GetIndexBuffer()->GetCount() : a_IndexCount;

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
