#pragma once

#include "Buffer/IndexBuffer.h"
#include "Buffer/VertexBuffer.h"

#include <memory>

namespace Muse
{
    class VertexArray
    {
    public:
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& a_VertexBuffer) = 0;
        virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& a_IndexBuffer) = 0;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

        static VertexArray* Create();
    };
}
