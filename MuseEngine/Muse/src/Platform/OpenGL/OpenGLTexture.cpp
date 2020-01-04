#include "MusePCH.h"
#include "OpenGLTexture.h"

#include "stb_image.h"
#include <glad/glad.h>
#include "Core/Utilities/Defines.h"

namespace Muse
{
    OpenGLTexture::OpenGLTexture(uint32_t a_Width, uint32_t a_Height)
        : m_Width(a_Width), m_Height(a_Height)
    {
        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;

        ASSERT_ENGINE(m_InternalFormat & m_DataFormat, "Format not supported!");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
        glTextureStorage2D(m_RendererId, 1, m_InternalFormat, m_Width, m_Height);

        glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    OpenGLTexture::OpenGLTexture(const std::string& a_Path)
        : m_Path(a_Path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = nullptr;
        {
            data = stbi_load(a_Path.c_str(), &width, &height, &channels, 0);
        }
        ASSERT_ENGINE(data, "Failed to load image!");
        m_Width = width;
        m_Height = height;

        if (channels == 4)
        {
            m_InternalFormat = GL_RGBA8;
            m_DataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            m_InternalFormat = GL_RGB8;
            m_DataFormat = GL_RGB;
        }

        ASSERT_ENGINE(m_InternalFormat & m_DataFormat, "Format not supported!");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
        glTextureStorage2D(m_RendererId, 1, m_InternalFormat, m_Width, m_Height);

        glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture::~OpenGLTexture()
    {
        glDeleteTextures(1, &m_RendererId);
    }

    void OpenGLTexture::SetData(void* a_Data, uint32_t a_Size)
    {
        uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;

        ASSERT_ENGINE(a_Size == m_Width * m_Height * bpp, "Data must be entire texture!");
        glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, a_Data);
    }

    void OpenGLTexture::Bind(uint32_t a_Slot) const
    {
        glBindTextureUnit(a_Slot, m_RendererId);
    }
}
