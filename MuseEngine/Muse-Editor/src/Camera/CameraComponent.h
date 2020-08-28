#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Muse
{
    struct CameraComponent
    {
        glm::mat4 m_ViewMatrix = glm::identity<glm::mat4>();
        glm::mat4 m_ProjectionMatrix = glm::identity<glm::mat4>();
        glm::mat4 m_ViewProjectionMatrix = glm::identity<glm::mat4>();
        float m_ZoomLevel = 1.0f;
        float m_AspectRatio = 1.6f;

        bool m_IsDirty = false;

        static CameraComponent* s_MainCamera;
    };
}