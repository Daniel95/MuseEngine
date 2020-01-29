#include "MusePCH.h"
#include "OpenGLFrameBuffer.h"
#include "glad/glad.h"
#include "Core/Instrumentor.h"

namespace Muse
{
    OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t a_Width, uint32_t a_Height, FramebufferFormat a_Format)
        : m_Width(a_Width), m_Height(a_Height), m_Format(a_Format)
    {
        MUSE_PROFILE_FUNCTION();

        glGenFramebuffers(1, &m_RendererId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

        Resize(a_Width, a_Height);
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        MUSE_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_RendererId);
    }

    void OpenGLFrameBuffer::Bind() const
    {
        MUSE_PROFILE_FUNCTION();

        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
        glViewport(0, 0, m_Width, m_Height);
    }

    void OpenGLFrameBuffer::Unbind() const
    {
        MUSE_PROFILE_FUNCTION();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::Resize(uint32_t a_Width, uint32_t a_Height)
    {
        if (m_Width == a_Width && m_Height == a_Height)
        {
            return;
        }

        m_Width = a_Width;
        m_Height = a_Height;

        if (m_RendererId)
        {
            glDeleteFramebuffers(1, &m_RendererId);
            glDeleteTextures(1, &m_ColorAttachment);
            glDeleteTextures(1, &m_DepthAttachment);
        }

        glGenFramebuffers(1, &m_RendererId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

        glGenTextures(1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);

        // TODO: Create Muse texture object based on format here
        if (m_Format == FramebufferFormat::RGBA16F)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, nullptr);
        }
        else if (m_Format == FramebufferFormat::RGBA8)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

        glGenTextures(1, &m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Width, m_Height, 0,
            GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
        );

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            LOG_ENGINE_ERROR("Framebuffer is incomplete!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::SetDataF(void* a_Data, uint32_t )
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, a_Data);
    }

    void OpenGLFrameBuffer::BindTexture(uint32_t a_Slot) const
    {
        glActiveTexture(GL_TEXTURE0 + a_Slot);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
    }
}
