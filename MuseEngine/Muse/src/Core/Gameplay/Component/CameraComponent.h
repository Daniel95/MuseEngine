#pragma once
#include "Component.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Muse
{
    class CameraComponent : public Component
    {
    public:
        CameraComponent();
        virtual ~CameraComponent() = default;

        void MakeEditorCamera() { m_IsEditorCamera = true; }
        bool IsEditorCamera() const { return m_IsEditorCamera; }

        void SetProjectionMatrix(float a_Left, float a_Right, float a_Bottom, float a_Top, float a_Near = -1.0, float a_Far = 1.0);
        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const;
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

        virtual void Update(float a_DeltaTime) override { RecalculateViewMatrix(); }

        static CameraComponent* GetMain() { return s_MainCamera; }

    private:
        glm::mat4 m_ViewMatrix = glm::identity<glm::mat4>();
        glm::mat4 m_ProjectionMatrix = glm::identity<glm::mat4>();
        glm::mat4 m_ViewProjectionMatrix = glm::identity<glm::mat4>();
        bool m_IsEditorCamera = false;
        glm::vec3 m_Position = glm::vec3(0, 0, 0);

        static CameraComponent* s_MainCamera;
        void RecalculateViewMatrix();
    };
}
