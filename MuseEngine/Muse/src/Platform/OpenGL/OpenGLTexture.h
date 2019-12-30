#pragma once
#include "Core/Renderer/Texture.h"

namespace Muse
{
    class OpenGLTexture : public Texture2D
    {
    public:
        OpenGLTexture(const std::string& a_Path);
        virtual ~OpenGLTexture();

        uint32_t GetWidth() const override { return m_Width; }
        uint32_t GetHeight() const override { return m_Height; }
        void Bind(uint32_t a_Slot = 0) const override;

    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        uint32_t m_RendererId;

    };
}
