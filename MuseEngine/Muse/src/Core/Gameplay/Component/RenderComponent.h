﻿#pragma once
#include "Component.h"

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
#include "Core/Renderer/RayTracing/Material/Material.h"

#include <memory>

namespace Muse
{
    struct Ray;
    struct RayHitData;
    struct GetColorParameters;
    class Shape;
    class Texture;
    class Shader;
    class BufferLayout;
    class VertexArray;
    class IndexBuffer;
    class VertexBuffer;

    class RenderComponent : public Component, public std::enable_shared_from_this<RenderComponent>
    {
        RTTR_ENABLE()
        RTTR_REGISTRATION_FRIEND

    public:
        RenderComponent() = default;
        virtual ~RenderComponent() = default;

        void SetMesh(float a_Vertices[], int a_VerticesCount, uint32_t a_Indices[], int a_IndicesCount, const BufferLayout& a_BufferLayout);
        void SetShader(std::shared_ptr<Shader> a_Shader) { m_Shader = a_Shader; }
        std::shared_ptr<Shader> GetShader() const { return m_Shader; }
        void SetTexture(std::shared_ptr<Texture> a_Texture) { m_Texture = a_Texture; }
        std::shared_ptr<Texture> GetTexture() const { return m_Texture; }
        std::shared_ptr<VertexArray> GetVA() const { return m_VA; }
        std::shared_ptr<Shape> GetShape() const { return m_Shape; }
        std::shared_ptr<const Material> GetMaterial() const { return m_Material; }
        void SetShape(std::shared_ptr<Shape> a_Material) { m_Shape = a_Material; }
        void SetMaterial(std::shared_ptr<Material> a_Material) { m_Material = a_Material; }

        virtual glm::vec3 GetColor(const glm::vec3& a_Point, std::shared_ptr<GetColorParameters> a_GetColorParameters) const;
        virtual bool CheckRayHit(RayHitData& a_RayHitData, const Ray& a_Ray) const;
        virtual glm::vec3 GetNormal(const glm::vec3& a_Point) const;

        virtual void OnUpdate(float a_DeltaTime) override;
        void OnEnable() override;
        void OnDisable() override;

        static const std::vector<std::shared_ptr<RenderComponent>>& GetAll() { return m_RenderComponents; }

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::make_nvp("Component", cereal::base_class<Component>(this)));
            ar(
                /*
                m_VB,
                m_IB,
                m_VA,
                m_Shader,
                m_Texture
                */
                test
            );
        }
    private:
        static std::vector<std::shared_ptr<RenderComponent>> m_RenderComponents;

        std::shared_ptr<VertexBuffer> m_VB;
        std::shared_ptr<IndexBuffer> m_IB;
        std::shared_ptr<VertexArray> m_VA;
        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<Texture> m_Texture;

        std::shared_ptr<Shape> m_Shape;
        std::shared_ptr<Material> m_Material;

        int test = 1;
    };
}

CEREAL_REGISTER_TYPE_WITH_NAME(Muse::RenderComponent, "RenderComponent")
CEREAL_REGISTER_POLYMORPHIC_RELATION(Muse::Component, Muse::RenderComponent)