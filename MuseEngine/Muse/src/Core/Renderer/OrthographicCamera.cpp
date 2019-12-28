#include "MusePCH.h"
#include "OrthographicCamera.h"

namespace Muse
{
    OrthographicCamera::OrthographicCamera(float a_Left, float a_right, float a_bottom, float a_top)
        : m_ProjectionMatrix(glm::ortho(a_Left, a_right, a_bottom, a_top, -1.0f, 1.0f))
    {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::SetPosition(const glm::vec3& a_Position)
    {
        m_Position = a_Position;
        RecalculateViewMatrix();
    }

    void OrthographicCamera::SetRotation(float a_Rotation)
    {
        m_Rotation = a_Rotation;
        RecalculateViewMatrix();
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
        transform = glm::rotate(transform, m_Rotation, glm::vec3(0, 0, 1));

        m_ViewMatrix = glm::inverse(transform);

        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}