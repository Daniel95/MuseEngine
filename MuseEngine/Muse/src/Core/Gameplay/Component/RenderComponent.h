#pragma once
#include "Component.h"

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
namespace Muse
{
    class Texture;
    class Shader;
    class BufferLayout;
    class VertexArray;
    class IndexBuffer;
    class VertexBuffer;

    class RenderComponent : public Component
    {
        RTTR_ENABLE()
        RTTR_REGISTRATION_FRIEND

    public:
        void SetMesh(float a_Vertices[], int a_VerticesCount, uint32_t a_Indices[], int a_IndicesCount, const BufferLayout& a_BufferLayout);
        void SetShader(std::shared_ptr<Shader> a_Shader) { m_Shader = a_Shader; }
        std::shared_ptr<Shader> GetShader() const { return m_Shader; }
        void SetTexture(std::shared_ptr<Texture> a_Texture) { m_Texture = a_Texture; }
        std::shared_ptr<Texture> GetTexture() const { return m_Texture; }

        virtual void OnUpdate(float a_DeltaTime) override;
        void OnEnable() override;
        void OnDisable() override;

        std::shared_ptr<VertexArray> GetVA() const { return m_VA; }
        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::make_nvp("Component", cereal::base_class<Component>(this)));
            ar(
                temp
            );
        }
    private:
        std::shared_ptr<VertexBuffer> m_VB;
        std::shared_ptr<IndexBuffer> m_IB;
        std::shared_ptr<VertexArray> m_VA;
        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<Texture> m_Texture;

        int temp;
    };
}

CEREAL_REGISTER_TYPE_WITH_NAME(Muse::RenderComponent, "RenderComponent")
CEREAL_REGISTER_POLYMORPHIC_RELATION(Muse::Component, Muse::RenderComponent)