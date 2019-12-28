#pragma once
#include <glm/gtc/matrix_transform.hpp>

namespace Muse
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float a_Left, float a_right, float a_bottom, float a_top);

        void SetPosition(const glm::vec3& a_Position);
        const glm::vec3& GetPosition() const { return m_Position; }

        void SetRotation(float a_Rotation);
        float GetRotation() const { return m_Rotation; }

        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    private:
        glm::mat4 m_ProjectionMatrix = glm::identity<glm::mat4>();
        glm::mat4 m_ViewMatrix = glm::identity<glm::mat4>();
        glm::mat4 m_ViewProjectionMatrix = glm::identity<glm::mat4>();

        glm::vec3 m_Position;
        float m_Rotation = 0.0f;

        void RecalculateViewMatrix();
    };
}