#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Muse
{
    class TransformComponent;

    class CameraComponent
    {
    public:
        CameraComponent();
        ~CameraComponent() = default;

        void SetProjection(float a_AspectRatio, float a_ZoomLevel);
        void SetProjectionMatrix(float a_Left, float a_Right, float a_Bottom, float a_Top, float a_Near = -1.0, float a_Far = 1.0);
        const glm::mat4& GetProjectionViewMatrix(TransformComponent& a_TransformComponent);

        void SetZoomLevel(float a_ZoomLevel) { SetProjection(m_AspectRatio, a_ZoomLevel); }
        float GetZoomLevel() { return m_ZoomLevel; }

        void SetAspectRatio(float a_AspectRatio) { SetProjection(a_AspectRatio, m_ZoomLevel); }
        float GetAspectRatio() { return m_AspectRatio; }

        void SetFixedAspectRatio(bool a_FixedAspectRatio) { m_FixedAspectRatio = a_FixedAspectRatio; }
        bool GetFixedAspectRatio() { return m_FixedAspectRatio; }

        void SetViewportSize(uint32_t a_Width, uint32_t a_Height);

        void SetOrthographicSize(float a_Size);
        float GetOrthographicSize() const { return m_OrthographicSize; }

        static CameraComponent* GetMain() { return s_MainCamera; };
        static void SetMain(CameraComponent* a_CameraComponent) { s_MainCamera = a_CameraComponent; };

    private:
        void RecalculateProjection();

        glm::mat4 m_ViewMatrix = glm::identity<glm::mat4>();
        glm::mat4 m_ProjectionMatrix = glm::identity<glm::mat4>();
        glm::mat4 m_ViewProjectionMatrix = glm::identity<glm::mat4>();
        float m_ZoomLevel = 1.0f;
        float m_AspectRatio = 1.6f;
        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

        bool m_IsDirty = true;
        bool m_FixedAspectRatio = false;

        static CameraComponent* s_MainCamera;
    };
}