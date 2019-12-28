#pragma once
#include "Component.h"

namespace Muse
{
    class CameraComponent : public Component
    {
    public:
        CameraComponent();
        virtual ~CameraComponent() = default;

        void MakeEditorCamera() { m_IsEditorCamera = true; }
        bool IsEditorCamera() const { return m_IsEditorCamera; }

    private:
        bool m_IsEditorCamera = false;

        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;

        glm::vec3 m_Position;
        float m_Rotation = 0.0f;
    };
}
