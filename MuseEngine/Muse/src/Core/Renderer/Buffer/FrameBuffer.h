#pragma once

#include <memory>

namespace Muse
{
    enum class FramebufferFormat
    {
        None = 0,
        RGBA8 = 1,
        RGBA16F = 2
    };

    class FrameBuffer
    {
    public:
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void Resize(uint32_t a_Width, uint32_t a_Height) = 0;
        virtual void SetDataF(void* a_Data, uint32_t a_Size) = 0;

        virtual void BindTexture(uint32_t a_Slot = 0) const = 0;

        virtual uint32_t GetRendererID() const = 0;
        virtual uint32_t GetColorAttachmentRendererID() const = 0;
        virtual uint32_t GetDepthAttachmentRendererID() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual uint32_t GetWidth() const = 0;

        static std::shared_ptr<FrameBuffer> Create(uint32_t a_Width, uint32_t a_Height, FramebufferFormat a_Format);

    };
}