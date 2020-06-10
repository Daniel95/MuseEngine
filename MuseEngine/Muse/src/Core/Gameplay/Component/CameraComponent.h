#pragma once
#include "Component.h"
#include <glm/gtc/matrix_transform.hpp>

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

#include <memory>

namespace Muse
{
    class CameraComponent : public Component
    {
        RTTR_ENABLE(Component)
        RTTR_REGISTRATION_FRIEND

    public:
        CameraComponent();
        virtual ~CameraComponent() = default;

        void MakeEditorCamera() { m_IsEditorCamera = true; }
        bool IsEditorCamera() const { return m_IsEditorCamera; }

        float GetZoomLevel() const { return m_ZoomLevel; }
        float GetAspectRatio() const { return m_AspectRatio; }
        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const;
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
        virtual void OnInit() override;

        void SetZoomLevel(float a_ZoomLevel) { SetProjection(GetAspectRatio(), a_ZoomLevel); }
        void SetAspectRatio(float a_AspectRatio) { SetProjection(a_AspectRatio, GetZoomLevel()); }
        void SetProjection(float a_AspectRatio, float a_ZoomLevel);
        void SetProjectionMatrix(float a_Left, float a_Right, float a_Bottom, float a_Top, float a_Near = -1.0, float a_Far = 1.0);
        void SetProjectionMatrix(const glm::mat4& a_ProjectionMatrix);

        static CameraComponent* GetMain() { return s_MainCamera; }

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::make_nvp("Component", cereal::base_class<Component>(this)));
            ar(
                m_ViewMatrix,
                m_ProjectionMatrix,
                m_ViewProjectionMatrix,
                m_IsEditorCamera,
                m_ZoomLevel,
                m_AspectRatio
            );
        }

    protected:
        virtual void OnUpdate(float a_DeltaTime) override { RecalculateViewMatrix(); }
        float m_AspectRatio = 1.6f;

    private:
        glm::mat4 m_ViewMatrix = glm::identity<glm::mat4>();
        glm::mat4 m_ProjectionMatrix = glm::identity<glm::mat4>();
        glm::mat4 m_ViewProjectionMatrix = glm::identity<glm::mat4>();
        bool m_IsEditorCamera = false;
        float m_ZoomLevel = 1.0f;

        static CameraComponent* s_MainCamera;
        void RecalculateViewMatrix();

    };
}

CEREAL_REGISTER_TYPE_WITH_NAME(Muse::CameraComponent, "CameraComponent")
CEREAL_REGISTER_POLYMORPHIC_RELATION(Muse::Component, Muse::CameraComponent)
