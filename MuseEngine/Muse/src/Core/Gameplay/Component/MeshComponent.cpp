#include "MusePCH.h"
#include "MeshComponent.h"

#include "Core/Renderer/Buffer/VertexBuffer.h"
#include "Core/Renderer/Buffer/IndexBuffer.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/Buffer/BufferLayout.h"

namespace Muse
{
    void MeshComponent::SetMesh(float a_Vertices[], int a_VerticesCount, uint32_t a_Indices[], int a_IndicesCount, const BufferLayout& a_BufferLayout)
    {
        m_VA.reset(Muse::VertexArray::Create());

        m_VB.reset(Muse::VertexBuffer::Create(a_Vertices, sizeof(float) * a_VerticesCount));
        m_VB->Bind();

        m_VB->SetLayout(a_BufferLayout);
        m_VA->AddVertexBuffer(m_VB);

        //const uint32_t count = sizeof(a_Indices) / sizeof(uint32_t);
        m_IB.reset(Muse::IndexBuffer::Create(a_Indices, a_IndicesCount));
        m_VA->SetIndexBuffer(m_IB);
    }

    void MeshComponent::OnEnable()
    {
    }

    void MeshComponent::OnDisable()
    {
    }
}
