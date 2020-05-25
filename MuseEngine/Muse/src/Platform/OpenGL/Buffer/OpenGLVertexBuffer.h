#pragma once

#include "Core/Renderer/Buffer/VertexBuffer.h"
#include "Core/Renderer/Buffer/BufferLayout.h"

namespace Muse
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(uint32_t a_Size);
        OpenGLVertexBuffer(float* a_Vertices, uint32_t a_Size);
        virtual ~OpenGLVertexBuffer();

        void Bind() const override;
        void Unbind() const override;

        virtual void SetData(const void* a_Data, uint32_t a_Size) const override;

        virtual const BufferLayout& GetLayout() const override { return m_Layout; }
        virtual void SetLayout(const BufferLayout& a_Layout) override { m_Layout = a_Layout; }
    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;

    };

}
