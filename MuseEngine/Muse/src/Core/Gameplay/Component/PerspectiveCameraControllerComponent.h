#pragma once
#include "Component.h"

#include <memory>

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
namespace Muse
{
    class CameraComponent;

    class PerspectiveCameraControllerComponent : public Component
    {
        RTTR_ENABLE()
            RTTR_REGISTRATION_FRIEND

    public:
        PerspectiveCameraControllerComponent();
        virtual ~PerspectiveCameraControllerComponent();
        virtual void OnInit() override;
        void OnUpdate(float a_DeltaTime) override;
        void SetMoveSpeed(float a_CameraMoveSpeed) { m_BaseMoveSpeed = a_CameraMoveSpeed; }
        float GetMoveSpeed() const { return m_BaseMoveSpeed; }
        void SetZoomSpeed(float a_ZoomSpeed) { m_ZoomSpeed = a_ZoomSpeed; }
        float GetZoomSpeed() const { return m_ZoomSpeed; }
        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::make_nvp("Component", cereal::base_class<Component>(this)));
            ar(
                m_BaseMoveSpeed,
                m_ZoomSpeed,
                m_MinZoomLevel
            );
        }
    private:
        float m_BaseMoveSpeed = 0.8f;
        float m_BaseRotateSpeed = 0.2f;
        float m_ZoomSpeed = 0.1f;
        float m_MinZoomLevel = 0.1f;
        std::shared_ptr<CameraComponent> m_CameraComponent = nullptr;

        void OnMouseScrolledEvent(float a_XOffset, float a_YOffset) const;
        void OnWindowResizeEvent(int a_Width, int a_Height) const;

    };
}

CEREAL_REGISTER_TYPE_WITH_NAME(Muse::PerspectiveCameraControllerComponent, "PerspectiveCameraControllerComponent")
CEREAL_REGISTER_POLYMORPHIC_RELATION(Muse::Component, Muse::PerspectiveCameraControllerComponent)