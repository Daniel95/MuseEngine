#pragma once
#include "Component.h"

namespace Muse
{
    class BufferLayout;
    class VertexArray;
    class IndexBuffer;
    class VertexBuffer;

    class MeshComponent : public Component
    {
    public:
        void SetMesh(float a_Vertices[], int a_VerticesCount, uint32_t a_Indices[], int a_IndicesCount, const BufferLayout& a_BufferLayout);

        void OnEnable() override;
        void OnDisable() override;

        std::shared_ptr<VertexArray> GetVA() const { return m_VA; }

    private:
        std::shared_ptr<VertexBuffer> m_VB;
        std::shared_ptr<IndexBuffer> m_IB;
        std::shared_ptr<VertexArray> m_VA;

    };
}

