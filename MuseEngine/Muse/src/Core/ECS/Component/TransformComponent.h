#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

#include "Core/ECS/Entity/Entity.h"

namespace Muse
{
    class TransformComponent
    {
    public:
        TransformComponent() = default;
        TransformComponent(const glm::vec3& a_LocalPosition);
        TransformComponent(const glm::vec3& a_LocalPosition, const glm::vec3& a_LocalScale);
        TransformComponent(const glm::vec3& a_LocalPosition, const glm::vec3& a_LocalScale, const glm::vec3& a_LocalRotation);
        ~TransformComponent() = default;
        void SetLocalPosition(const glm::vec3& a_Position);
        void SetLocalPosition(const glm::vec2& a_Position);
        void TranslateLocal(const glm::vec3& a_Movement);
        void TranslateLocal(const glm::vec2& a_Movement);

        void SetLocalScale(const glm::vec3& a_Scale);
        void ScaleLocal(const glm::vec3& a_Scaling);

        void SetLocalRotation(const glm::vec3& a_Rotation);
        const glm::vec3& GetWorldRotation();

        void SetLookAt(glm::vec3 a_Target);

        bool GetDirty() { return m_Dirty; }

        glm::vec3 InverseTransformPoint(const glm::mat4& a_Model, const glm::vec3& a_WorldPoint); //World to local point 
        glm::vec3 InverseTransformVector(const glm::mat4& a_Model, const glm::vec3& a_WorldVector); //World to local vector
        glm::vec3 TransformPoint(const glm::mat4& a_Model, const glm::vec3& a_LocalPoint); //Local to world point
        glm::vec3 TransformVector(const glm::mat4& a_Model, const glm::vec3& a_LocalVector); //Local to world vector

        glm::vec3 GetRight() { return TransformVector(GetWorldModelMatrix(), glm::vec3(1, 0, 0)); }
        glm::vec3 GetUp() { return TransformVector(GetWorldModelMatrix(), glm::vec3(0, 1, 0)); }
        glm::vec3 GetForward() { return TransformVector(GetWorldModelMatrix(), glm::vec3(0, 0, 1)); }

        glm::vec3 GetWorldPosition();
        glm::vec3 GetWorldScale();
        glm::mat4 CalculateWorldRotationMatrix();

        const glm::mat4& GetWorldModelMatrix();
        glm::vec3 m_LocalPosition = glm::vec3(0);

    private:
        glm::vec3 m_LocalScale = glm::vec3(1);
        //Radians
        glm::vec3 m_LocalRotation = glm::vec3(0);

        glm::mat4 m_WorldModelMatrix = glm::identity<glm::mat4>();

        std::vector<Entity> m_Children;
        Entity* m_Parent = nullptr;
        bool m_Dirty = true;
    };
}