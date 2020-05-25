#pragma once

namespace Muse
{
    class ViewPort
    {
    public:
        static void Render(uint32_t a_ColorAttachmentRendererId);

        static int GetWidth() { return m_Width; }
        static int GetHeight() { return m_Height; }
        static float GetAspectRatio() { return m_AspectRatio;  }

    private:
        static int m_Width;
        static int m_Height;
        static float m_AspectRatio;

    };
}