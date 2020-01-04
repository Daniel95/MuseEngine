#pragma once
#include "Core/Renderer/Texture.h"

#include <glad/glad.h>

namespace Muse
{
    class OpenGLTexture : public Texture2D
    {
    public:
        OpenGLTexture(uint32_t a_Width, uint32_t a_Height);
        OpenGLTexture(const std::string& a_Path);
        virtual ~OpenGLTexture();

        virtual void SetData(void* a_Data, uint32_t a_Size) override;

        uint32_t GetWidth() const override { return m_Width; }
        uint32_t GetHeight() const override { return m_Height; }
        void Bind(uint32_t a_Slot = 0) const override;

    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        uint32_t m_RendererId;
        GLenum m_InternalFormat = 0;
        GLenum m_DataFormat = 0;
    };
}
