#pragma once
#include "Component.h"
#include <glm/gtc/matrix_transform.hpp>

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

        void SetProjection(float a_AspectRatio, float a_ZoomLevel);
        void SetProjectionMatrix(float a_Left, float a_Right, float a_Bottom, float a_Top, float a_Near = -1.0, float a_Far = 1.0);
        void SetProjectionMatrix(const glm::mat4& a_ProjectionMatrix);

        static CameraComponent* GetMain() { return s_MainCamera; }

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(m_ViewMatrix,
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

