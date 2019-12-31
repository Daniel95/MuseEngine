#pragma once

#include <memory>

namespace Muse
{
    class BufferLayout;

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual const BufferLayout& GetLayout() const = 0;
        virtual void SetLayout(const BufferLayout& layout) = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static std::shared_ptr<VertexBuffer> Create(float* a_Vertices, uint32_t a_Size);
    };
}
