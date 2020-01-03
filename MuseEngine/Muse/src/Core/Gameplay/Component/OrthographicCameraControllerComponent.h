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
        void SetCameraMoveSpeed(float a_CameraMoveSpeed) { m_CameraMoveSpeed = a_CameraMoveSpeed; }
        float GetCameraMoveSpeed() const { return m_CameraMoveSpeed; }

    private:
        float m_CameraMoveSpeed = 0.3f;
        CameraComponent* m_CameraComponent = nullptr;

        void OnMouseScrolledEvent(float a_XOffset, float a_YOffset);
        void OnWindowResizeEvent(int a_Width, int a_Height);

    };
}
