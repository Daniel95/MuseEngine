#pragma once
#include "CameraComponent.h"

namespace Muse
{
    class OrthographicCameraControllerComponent : public CameraComponent
    {
    public:
        void Update(float a_DeltaTime) override;
        void SetCameraMoveSpeed(float a_CameraMoveSpeed) { m_CameraMoveSpeed = a_CameraMoveSpeed; }
        float GetCameraMoveSpeed() const { return m_CameraMoveSpeed; }

    private:
        float m_CameraMoveSpeed = 0.3f;

    };
}
