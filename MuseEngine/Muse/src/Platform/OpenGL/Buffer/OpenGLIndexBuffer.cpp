#include "MusePCH.h"
#include "OpenGLIndexBuffer.h"

#include "glad/glad.h"

namespace Muse
{
    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* a_Indices, uint32_t a_Count)
        : m_Count(a_Count)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_Count * sizeof(uint32_t), a_Indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
