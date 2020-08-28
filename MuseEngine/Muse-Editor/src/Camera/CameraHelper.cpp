#include "CameraHelper.h"
#include "Core/Instrumentor.h"

void Muse::CameraHelper::SetProjection(CameraComponent& a_CameraComponent, float a_AspectRatio, float a_ZoomLevel)
{
    MUSE_PROFILE_FUNCTION();

    a_CameraComponent.m_AspectRatio = a_AspectRatio;
    a_CameraComponent.m_ZoomLevel = a_ZoomLevel;
    SetProjectionMatrix(a_CameraComponent, -a_AspectRatio * a_ZoomLevel, a_AspectRatio * a_ZoomLevel, -a_ZoomLevel, a_ZoomLevel);
}

void Muse::CameraHelper::SetProjectionMatrix(CameraComponent& a_CameraComponent, float a_Left, float a_Right, float a_Bottom, float a_Top, float a_Near, float a_Far)
{
    MUSE_PROFILE_FUNCTION();

    a_CameraComponent.m_ProjectionMatrix = glm::ortho(a_Left, a_Right, a_Bottom, a_Top, a_Near, a_Far);
}