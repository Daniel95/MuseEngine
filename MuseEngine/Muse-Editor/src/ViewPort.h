#pragma once

namespace Muse
{
    class ViewPort
    {
    public:
        static void Render(uint32_t a_ColorAttachmentRendererId, glm::vec2& a_ViewPortSize, float& a_AspectRatio);

    };
}