#pragma once
#include "Component.h"

namespace Muse
{
    class CameraComponent;

    class OrthographicCameraControllerComponent : public Component
    {
    public:
        OrthographicCameraControllerComponent();
        virtual ~OrthographicCameraControllerComponent();
        virtual void OnInit() override;
        void OnUpdate(float a_DeltaTime) override;
        void SetMoveSpeed(float a_CameraMoveSpeed) { m_MoveSpeed = a_CameraMoveSpeed; }
        float GetMoveSpeed() const { return m_MoveSpeed; }
        void SetZoomSpeed(float a_ZoomSpeed) { m_ZoomSpeed = a_ZoomSpeed; }
        float GetZoomSpeed() const { return m_ZoomSpeed; }

    private:
        float m_MoveSpeed = 0.3f;
        float m_ZoomSpeed = 0.3f;
        CameraComponent* m_CameraComponent = nullptr;

        void OnMouseScrolledEvent(float a_XOffset, float a_YOffset);
        void OnWindowResizeEvent(int a_Width, int a_Height);

    };
}
