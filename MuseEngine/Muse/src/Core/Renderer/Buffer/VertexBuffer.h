#pragma once

namespace Muse
{
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer();

        virtual void SetData();

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static VertexBuffer* Create(float* a_Vertices, uint32_t a_Size);
    };
}
