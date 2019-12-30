#include "MusePCH.h"
#include "OpenGLTexture.h"

#include "stb_image.h"
#include <glad/glad.h>
#include "Core/Utilities/Defines.h"

namespace Muse
{
    OpenGLTexture::OpenGLTexture(const std::string& a_Path)
        : m_Path(a_Path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(a_Path.c_str(), &width, &height, &channels, 0);

        ASSERT(data, "Failed to load image!");

        m_Width = width;
        m_Height = height;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
        glTextureStorage2D(m_RendererId, 1, GL_RGB8, m_Width, m_Height);

        glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture::~OpenGLTexture()
    {
        glDeleteTextures(1, &m_RendererId);
    }

    void OpenGLTexture::Bind(uint32_t a_Slot) const
    {
        glBindTextureUnit(a_Slot, m_RendererId);
    }
}
