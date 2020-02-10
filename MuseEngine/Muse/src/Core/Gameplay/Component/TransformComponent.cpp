#include "MusePCH.h"

#include "Core/Gameplay/Component/TransformComponent.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/transform.hpp>
#include "Core/Instrumentor.h"

namespace Muse
{
    void TransformComponent::SetPosition(const glm::vec3& a_Position)
    {
		MUSE_PROFILE_FUNCTION();

		m_DirtyPosition = m_DirtyModel = true;
		m_Position = a_Position;
    }

    void TransformComponent::SetPosition(const glm::vec2& a_Position)
    {
		MUSE_PROFILE_FUNCTION();

		m_DirtyPosition = m_DirtyModel = true;
		m_Position = glm::vec3(a_Position.x, a_Position.y, m_Position.z);
    }

    void TransformComponent::Move(const glm::vec3& a_Movement)
    {
		MUSE_PROFILE_FUNCTION();

		m_DirtyPosition = m_DirtyModel = true;
		m_Position += a_Movement;
    }

    void TransformComponent::Move(const glm::vec2& a_Movement)
    {
		MUSE_PROFILE_FUNCTION();

		m_DirtyPosition = m_DirtyModel = true;
		m_Position += glm::vec3(a_Movement.x, a_Movement.y, 0);
    }

    void TransformComponent::SetScale(const glm::vec3& a_Scale)
    {
		MUSE_PROFILE_FUNCTION();

		m_DirtyScale = m_DirtyModel = true;
		m_Scale = a_Scale;
    }

    void TransformComponent::SetScale(const glm::vec2& a_Scale)
    {
		MUSE_PROFILE_FUNCTION();

		m_DirtyScale = m_DirtyModel = true;
		m_Scale = glm::vec3(a_Scale.x, a_Scale.y, m_Scale.z);;
    }

    void TransformComponent::SetRotation(const glm::vec3& a_Rotation)
    {
		MUSE_PROFILE_FUNCTION();

		m_DirtyRotation = m_DirtyModel = true;
		m_Rotation = a_Rotation;
    }

	void TransformComponent::Rotate(const glm::vec3& a_Rotation)
    {
		MUSE_PROFILE_FUNCTION();

		m_DirtyRotation = m_DirtyModel = true;
		m_Rotation += a_Rotation;
    }

    void TransformComponent::SetRotationQuat(const glm::quat& a_Rotation)
    {
		MUSE_PROFILE_FUNCTION();

		m_DirtyRotation = m_DirtyModel = true;
		m_RotationQuaternion = a_Rotation;
    }

	const glm::mat4& TransformComponent::GetTranslationMatrix()
	{
		MUSE_PROFILE_FUNCTION();

		if (m_DirtyPosition)
		{
			m_TranslationMatrix = glm::translate(m_Position);
			m_DirtyPosition = false;
		}

		return m_TranslationMatrix;
	}

	const glm::mat4& TransformComponent::GetRotationMatrix()
	{
		MUSE_PROFILE_FUNCTION();

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
		MUSE_PROFILE_FUNCTION();

		if (m_DirtyScale)
		{
			m_ScaleMatrix = glm::scale(m_Scale);

			m_DirtyScale = false;
		}

		return m_ScaleMatrix;
	}

	const glm::mat4& TransformComponent::GetModelMatrix()
	{
		MUSE_PROFILE_FUNCTION();

		if (m_DirtyModel)
		{
			m_ModelMatrix = GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();

			m_DirtyModel = false;
		}

		return m_ModelMatrix;
	}

	glm::vec3 TransformComponent::InverseTransformPoint(const glm::vec3& worldPoint)
	{
		MUSE_PROFILE_FUNCTION();

		const glm::vec4 transformed = glm::inverse(GetModelMatrix()) * glm::vec4(worldPoint, 1);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformComponent::InverseTransformVector(const glm::vec3& worldVector)
	{
		MUSE_PROFILE_FUNCTION();

		const glm::vec4 transformed = glm::inverse(GetModelMatrix()) * glm::vec4(worldVector, 0);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformComponent::TransformPoint(const glm::vec3& localPoint)
	{
		MUSE_PROFILE_FUNCTION();

		const glm::vec4 transformed = GetModelMatrix() * glm::vec4(localPoint, 1);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformComponent::TransformVector(const glm::vec3& localVector)
	{
		MUSE_PROFILE_FUNCTION();

		const glm::vec4 transformed = GetModelMatrix() * glm::vec4(localVector, 0);
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

	rttr::registration::class_<glm::vec4>("vec4")
		.constructor<>()
		(
			rttr::policy::ctor::as_object
			)
		.property("x", &glm::vec4::x)
		.property("y", &glm::vec4::y)
		.property("z", &glm::vec4::z)
		.property("z", &glm::vec4::w);

	rttr::registration::class_<glm::mat4>("mat4")
		.constructor<>()
		(
			rttr::policy::ctor::as_object
			)
		.property("value", &glm::mat4::value);
}