#include "MusePCH.h"
#include "OpenGLVertexBuffer.h"

#include "glad/glad.h"

namespace Muse
{
    /*
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* a_Vertices, uint32_t a_Size)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, a_Size, a_Vertices, GL_STATIC_DRAW);
    }
    */

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* a_Vertices, uint32_t a_Size)
    {
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
