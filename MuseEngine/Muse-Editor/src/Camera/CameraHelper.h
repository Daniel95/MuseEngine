#pragma once

#include "CameraComponent.h"

namespace Muse
{
    class CameraHelper
    {
        static void SetProjection(CameraComponent& a_CameraComponent, float a_AspectRatio, float a_ZoomLevel);
        static void SetProjectionMatrix(CameraComponent& a_CameraComponent, float a_Left, float a_Right, float a_Bottom, float a_Top, float a_Near = -1.0, float a_Far = 1.0);

        static void SetZoomLevel(CameraComponent& a_CameraComponent, float a_ZoomLevel) { SetProjection(a_CameraComponent, a_CameraComponent.m_AspectRatio, a_ZoomLevel); }
        static void SetAspectRatio(CameraComponent& a_CameraComponent, float a_AspectRatio) { SetProjection(a_CameraComponent, a_AspectRatio, a_CameraComponent.m_ZoomLevel); }
    };
}
