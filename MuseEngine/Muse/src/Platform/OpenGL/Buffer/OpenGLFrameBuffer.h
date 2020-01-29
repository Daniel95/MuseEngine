﻿#pragma once
#include "Core/Renderer/Buffer/FrameBuffer.h"

namespace Muse
{
    class OpenGLFrameBuffer : public FrameBuffer
    {
    public:
        OpenGLFrameBuffer(uint32_t a_Width, uint32_t a_Height, FramebufferFormat a_Format);
        virtual ~OpenGLFrameBuffer();

        void Bind() const override;
        void Unbind() const override;

        void Resize(uint32_t a_Width, uint32_t a_Height) override;
        virtual void SetDataF(void* a_Data, uint32_t a_Size) override;
        void BindTexture(uint32_t a_Slot) const override;
        uint32_t GetRendererID() const override { return m_RendererId; }
        uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
        uint32_t GetDepthAttachmentRendererID() const override { return m_DepthAttachment; }
        virtual uint32_t GetHeight() const override { return m_Height; }
        virtual uint32_t GetWidth() const override { return m_Width; }

    private:
        uint32_t m_RendererId;
        uint32_t m_Width, m_Height;
        FramebufferFormat m_Format;

        uint32_t m_ColorAttachment, m_DepthAttachment;
    };
}
