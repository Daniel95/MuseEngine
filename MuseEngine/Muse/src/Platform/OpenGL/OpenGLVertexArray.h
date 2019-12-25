#pragma once

#include "Core/Renderer/VertexArray.h"

namespace Muse
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray() = default;

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& a_VertexBuffer) override;
        virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& a_IndexBuffer) override;

    private:
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
        uint32_t m_RendererId;

    };
}