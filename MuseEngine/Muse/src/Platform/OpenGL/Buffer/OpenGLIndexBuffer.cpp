#include "MusePCH.h"
#include "OpenGLIndexBuffer.h"

#include "glad/glad.h"
#include "Core/Instrumentor.h"

namespace Muse
{
    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* a_Indices, uint32_t a_Count)
        : m_Count(a_Count)
    {
        MUSE_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_Count * sizeof(uint32_t), a_Indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        MUSE_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        MUSE_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        MUSE_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
