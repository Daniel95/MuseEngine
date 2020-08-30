#include "MusePCH.h"
#include "TransformComponent.h"
#include "Core/Instrumentor.h"

#define GLM_ENABLE_EXPERIMENTAL
#undef countof
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#undef countof

namespace Muse
{
	void TransformComponent::SetLocalPosition(const glm::vec3& a_Position)
	{
		MUSE_PROFILE_FUNCTION();

		m_Dirty = true;
		m_LocalPosition = a_Position;
	}

	void TransformComponent::SetLocalPosition(const glm::vec2& a_Position)
	{
		MUSE_PROFILE_FUNCTION();

		m_Dirty = true;
		m_LocalPosition = glm::vec3(a_Position.x, a_Position.y, m_LocalPosition.z);
	}

	void TransformComponent::TranslateLocal(const glm::vec3& a_Movement)
	{
		MUSE_PROFILE_FUNCTION();

		m_Dirty = true;
		m_LocalPosition += a_Movement;
	}

	void TransformComponent::TranslateLocal(const glm::vec2& a_Movement)
	{
		MUSE_PROFILE_FUNCTION();

		m_Dirty = true;
		m_LocalPosition += glm::vec3(a_Movement.x, a_Movement.y, 0);
	}

	void TransformComponent::SetLocalScale(const glm::vec3& a_Scale)
	{
		MUSE_PROFILE_FUNCTION();

		m_Dirty = true;
		m_LocalScale = a_Scale;
	}

	void TransformComponent::ScaleLocal(const glm::vec3& a_Scaling)
	{
		MUSE_PROFILE_FUNCTION();

		m_Dirty = true;
		m_LocalScale += a_Scaling;
	}

	void TransformComponent::SetLocalRotation(const glm::vec3& a_Rotation)
	{
		MUSE_PROFILE_FUNCTION();

		m_Dirty = true;
		m_LocalRotation = a_Rotation;
	}

	void TransformComponent::SetLookAt(glm::vec3 a_Target)
	{
		MUSE_PROFILE_FUNCTION();

		const glm::vec3& position = m_LocalPosition;

		//TODO: use worldPosition
		if (position == a_Target) { return; }

		glm::vec3 direction = glm::normalize(position - a_Target);
		glm::mat4 rotationMatrix = glm::inverse(glm::lookAt(position, position + direction, glm::vec3(0, 1, 0)));
		//Reset position because lookat also includes the position, we don't need it for rotationMatrix
		rotationMatrix[3][0] = 0;
		rotationMatrix[3][1] = 0;
		rotationMatrix[3][2] = 0;

		glm::quat orientation = glm::toQuat(rotationMatrix);
		orientation = glm::conjugate(orientation);

		m_LocalRotation = glm::eulerAngles(orientation);
		m_LocalRotation.y = -m_LocalRotation.y;
		m_LocalRotation.x = -m_LocalRotation.x;

		m_Dirty = true;
	}

	glm::vec3 TransformComponent::InverseTransformPoint(const glm::mat4& a_Model, const glm::vec3& a_WorldPoint)
	{
		MUSE_PROFILE_FUNCTION();

		const glm::vec4 transformed = glm::inverse(a_Model) * glm::vec4(a_WorldPoint, 1);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformComponent::InverseTransformVector(const glm::mat4& a_Model, const glm::vec3& a_WorldVector)
	{
		MUSE_PROFILE_FUNCTION();

		const glm::vec4 transformed = glm::inverse(a_Model) * glm::vec4(a_WorldVector, 0);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformComponent::TransformPoint(const glm::mat4& a_Model, const glm::vec3& a_LocalPoint)
	{
		MUSE_PROFILE_FUNCTION();

		const glm::vec4 transformed = a_Model * glm::vec4(a_LocalPoint, 1);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformComponent::TransformVector(const glm::mat4& a_Model, const glm::vec3& a_LocalVector)
	{
		MUSE_PROFILE_FUNCTION();

		const glm::vec4 transformed = a_Model * glm::vec4(a_LocalVector, 0);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformComponent::GetWorldPosition()
	{
		MUSE_PROFILE_FUNCTION();

		//if (parent == nullptr)
		//{
		return m_LocalPosition;
		//}

		//return TransformPoint(*parent, m_LocalPosition);
	}

	glm::vec3 TransformComponent::GetWorldScale()
	{
		MUSE_PROFILE_FUNCTION();

		//if (parent == nullptr)
		//{
		return m_LocalScale;
		//}

		//return TransformVector(*parent, m_LocalScale);
	}

	glm::mat4 TransformComponent::CalculateWorldRotationMatrix()
	{
		MUSE_PROFILE_FUNCTION();

		glm::vec4 rotation = glm::vec4(m_LocalRotation, 0);

		/*
		if (parent != nullptr)
		{
			rotation = CalculateWorldRotationMatrix(*parent) * glm::vec4(m_LocalPosition, 0);
		}
		*/

		glm::mat4 worldRotationMatrix = glm::rotate(rotation.x, glm::vec3(1, 0, 0));
		worldRotationMatrix = glm::rotate(worldRotationMatrix, rotation.y, glm::vec3(0, 1, 0));
		worldRotationMatrix = glm::rotate(worldRotationMatrix, rotation.z, glm::vec3(0, 0, 1));

		return worldRotationMatrix;
	}

	const glm::mat4& TransformComponent::GetWorldModelMatrix()
	{
		MUSE_PROFILE_FUNCTION();

		glm::mat4 translation = glm::translate(GetWorldPosition());
		glm::mat4 rotation = CalculateWorldRotationMatrix();
		glm::mat4 scale = glm::scale(GetWorldScale());

		if (m_Dirty)
		{
			m_WorldModelMatrix = glm::translate(GetWorldPosition()) * CalculateWorldRotationMatrix() * glm::scale(GetWorldScale());
			m_Dirty = false;
		}

		return m_WorldModelMatrix;
	}
}