#pragma once
#include "Component.h"

namespace Muse
{
    class Shader;
    class BufferLayout;
    class VertexArray;
    class IndexBuffer;
    class VertexBuffer;

    class RenderComponent : public Component
    {
    public:
        void SetMesh(float a_Vertices[], int a_VerticesCount, uint32_t a_Indices[], int a_IndicesCount, const BufferLayout& a_BufferLayout);
        void SetShader(std::shared_ptr<Shader> a_Shader) { m_Shader = a_Shader; }
        std::shared_ptr<Shader> GetShader() const { return m_Shader; }


        virtual void Update(float a_DeltaTime) override;
        void OnEnable() override;
        void OnDisable() override;

        std::shared_ptr<VertexArray> GetVA() const { return m_VA; }

    private:
        std::shared_ptr<VertexBuffer> m_VB;
        std::shared_ptr<IndexBuffer> m_IB;
        std::shared_ptr<VertexArray> m_VA;
        std::shared_ptr<Shader> m_Shader;

    };
}

