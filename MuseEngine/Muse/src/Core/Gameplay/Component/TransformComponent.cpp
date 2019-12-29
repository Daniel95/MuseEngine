#include "MusePCH.h"

#include "Core/Gameplay/Component/TransformComponent.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/transform.hpp>

namespace Muse
{
    void TransformComponent::SetPosition(const glm::vec3& a_Position)
    {
		m_DirtyPosition = m_DirtyModel = true;
		m_Position = a_Position;
    }

    void TransformComponent::SetPosition(const glm::vec2& a_Position)
    {
		m_DirtyPosition = m_DirtyModel = true;
		m_Position = glm::vec3(a_Position.x, a_Position.y, m_Position.z);
    }

    void TransformComponent::Move(const glm::vec3& a_Movement)
    {
		m_DirtyPosition = m_DirtyModel = true;
		m_Position += a_Movement;
    }

    void TransformComponent::Move(const glm::vec2& a_Movement)
    {
		m_DirtyPosition = m_DirtyModel = true;
		m_Position += glm::vec3(a_Movement.x, a_Movement.y, 0);
    }

    void TransformComponent::SetScale(const glm::vec3& a_Scale)
    {
		m_DirtyScale = m_DirtyModel = true;
		m_Scale = a_Scale;
    }

    void TransformComponent::SetScale(const glm::vec2& a_Scale)
    {
		m_DirtyScale = m_DirtyModel = true;
		m_Scale = glm::vec3(a_Scale.x, a_Scale.y, m_Scale.z);;
    }

    void TransformComponent::SetRotation(const glm::vec3& a_Rotation)
    {
		m_DirtyScale = m_DirtyModel = true;
		m_Rotation = a_Rotation;
    }

    void TransformComponent::SetRotationQuat(const glm::quat& a_Rotation)
    {
		m_DirtyScale = m_DirtyModel = true;
		m_RotationQuaternion = a_Rotation;
    }

	const glm::mat4& TransformComponent::GetTranslationMatrix()
	{
		if (m_DirtyPosition)
		{
			m_TranslationMatrix = glm::translate(m_Position);
			m_DirtyPosition = false;
		}

		return m_TranslationMatrix;
	}

	const glm::mat4& TransformComponent::GetRotationMatrix()
	{
		if (m_DirtyRotation)
		{
			m_RotationMatrix = glm::rotate(m_Rotation.x, glm::vec3(1, 0, 0));
			m_RotationMatrix = glm::rotate(m_RotationMatrix, m_Rotation.y, glm::vec3(0, 1, 0));
			m_RotationMatrix = glm::rotate(m_RotationMatrix, m_Rotation.z, glm::vec3(0, 0, 1));

			m_DirtyRotation = false;
		}

		return m_RotationMatrix;
	}

	const glm::mat4& TransformComponent::GetScaleMatrix()
	{
		if (m_DirtyScale)
		{
			m_ScaleMatrix = glm::scale(m_Scale);

			m_DirtyScale = false;
		}

		return m_ScaleMatrix;
	}

	const glm::mat4& TransformComponent::GetModelMatrix()
	{
		if (m_DirtyModel)
		{
			m_ModelMatrix = GetScaleMatrix() * GetRotationMatrix() * GetTranslationMatrix();
		}

		return m_ModelMatrix;
	}

	glm::vec3 TransformComponent::InverseTransformPoint(const glm::vec3& worldPoint)
	{
		const glm::vec4 transformed = glm::inverse(GetModelMatrix()) * glm::vec4(worldPoint.x, worldPoint.y, worldPoint.z, 1);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformComponent::InverseTransformVector(const glm::vec3& worldVector)
	{
		const glm::vec4 transformed = glm::inverse(GetModelMatrix()) * glm::vec4(worldVector.x, worldVector.y, worldVector.z, 0);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformComponent::TransformPoint(const glm::vec3& localPoint)
	{
		const glm::vec4 transformed = GetModelMatrix() * glm::vec4(localPoint.x, localPoint.y, localPoint.z, 1);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformComponent::TransformVector(const glm::vec3& localVector)
	{
		const glm::vec4 transformed = GetModelMatrix() * glm::vec4(localVector.x, localVector.y, localVector.z, 0);
		return glm::vec3(transformed);
	}
}

RTTR_REGISTRATION
{
        rttr::registration::class_<glm::vec2>("vec2")
        .constructor<>()
        (
            rttr::policy::ctor::as_object
        )
        .property("x", &glm::vec2::x)
        .property("y", &glm::vec2::y);

    rttr::registration::class_<glm::vec3>("vec3")
    .constructor<>()
    (
        rttr::policy::ctor::as_object
        )
        .property("x", &glm::vec3::x)
        .property("y", &glm::vec3::y)
        .property("z", &glm::vec3::z);

    /*

	rttr::registration::class_<Engine::TransformComponent>("TransformComponent")
		.constructor<>()
		(
			rttr::policy::ctor::as_raw_ptr
		)
		.property("Position", &Engine::TransformComponent::m_Position)
		.property("Scale", &Engine::TransformComponent::m_Scale)
		.property("Rotation", &Engine::TransformComponent::m_RotationQuaternion);

	rttr::registration::class_<DXS::Quaternion>("Quaternion")
		.constructor<>()
		(
			rttr::policy::ctor::as_object
		)
		.property("x", &DXS::Quaternion::x)
		.property("y", &DXS::Quaternion::y)
		.property("z", &DXS::Quaternion::z)
		.property("w", &DXS::Quaternion::w);

	rttr::registration::class_<DXS::Vector3>("Vector3")
		.constructor<>()
		(
			rttr::policy::ctor::as_object
		)
		.property("x", &DXS::Vector3::x)
		.property("y", &DXS::Vector3::y)
		.property("z", &DXS::Vector3::z);
    */
}