#pragma once

#include "Core/Renderer/Buffer/IndexBuffer.h"

namespace Muse
{
    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* a_Indices, uint32_t a_Size);
        virtual ~OpenGLIndexBuffer();

        void Bind() const override;
        void Unbind() const  override;

        virtual uint32_t GetCount() const override { return m_Count; };

    private:
        uint32_t m_RendererID;
        uint32_t m_Count;

    };

}
