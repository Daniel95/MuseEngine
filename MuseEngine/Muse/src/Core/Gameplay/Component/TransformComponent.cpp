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
		m_LocalPosition = a_Position;
    }

    void TransformComponent::SetPosition(const glm::vec2& a_Position)
    {
		MUSE_PROFILE_FUNCTION();

		m_DirtyPosition = m_DirtyModel = true;
		m_LocalPosition = glm::vec3(a_Position.x, a_Position.y, m_LocalPosition.z);
    }

    void TransformComponent::Move(const glm::vec3& a_Movement)
    {
		MUSE_PROFILE_FUNCTION();

		m_DirtyPosition = m_DirtyModel = true;
		m_LocalPosition += a_Movement;
    }

    void TransformComponent::Move(const glm::vec2& a_Movement)
    {
		MUSE_PROFILE_FUNCTION();

		m_DirtyPosition = m_DirtyModel = true;
		m_LocalPosition += glm::vec3(a_Movement.x, a_Movement.y, 0);
    }

    void TransformComponent::SetScale(const glm::vec3& a_Scale)
    {
		MUSE_PROFILE_FUNCTION();

		m_DirtyScale = m_DirtyModel = true;
		m_LocalScale = a_Scale;
    }

    void TransformComponent::SetScale(const glm::vec2& a_Scale)
    {
		MUSE_PROFILE_FUNCTION();

		m_DirtyScale = m_DirtyModel = true;
		m_LocalScale = glm::vec3(a_Scale.x, a_Scale.y, m_LocalScale.z);;
    }

    void TransformComponent::SetRotation(const glm::vec3& a_Rotation)
    {
		MUSE_PROFILE_FUNCTION();

		m_DirtyRotation = m_DirtyModel = true;
		m_LocalRotation = a_Rotation;
    }

	void TransformComponent::Rotate(const glm::vec3& a_Rotation)
    {
		MUSE_PROFILE_FUNCTION();

		m_DirtyRotation = m_DirtyModel = true;
		m_LocalRotation += a_Rotation;
    }

    void TransformComponent::SetRotationQuat(const glm::quat& a_Rotation)
    {
		MUSE_PROFILE_FUNCTION();

		m_DirtyRotation = m_DirtyModel = true;
		m_RotationQuaternion = a_Rotation;
    }

	const glm::mat4& TransformComponent::GetLocalTranslationMatrix()
	{
		MUSE_PROFILE_FUNCTION();

		if (m_DirtyPosition)
		{
			m_LocalTranslationMatrix = glm::translate(m_LocalPosition);
			m_DirtyPosition = false;
		}

		return m_LocalTranslationMatrix;
	}

	const glm::mat4& TransformComponent::GetLocalRotationMatrix()
	{
		MUSE_PROFILE_FUNCTION();

		if (m_DirtyRotation)
		{
			m_LocalRotationMatrix = glm::rotate(m_LocalRotation.x, glm::vec3(1, 0, 0));
			m_LocalRotationMatrix = glm::rotate(m_LocalRotationMatrix, m_LocalRotation.y, glm::vec3(0, 1, 0));
			m_LocalRotationMatrix = glm::rotate(m_LocalRotationMatrix, m_LocalRotation.z, glm::vec3(0, 0, 1));

			m_DirtyRotation = false;
		}

		return m_LocalRotationMatrix;
	}

	const glm::mat4& TransformComponent::GetLocalScaleMatrix()
	{
		MUSE_PROFILE_FUNCTION();

		if (m_DirtyScale)
		{
			m_LocalScaleMatrix = glm::scale(m_LocalScale);

			m_DirtyScale = false;
		}

		return m_LocalScaleMatrix;
	}

	const glm::mat4& TransformComponent::GetLocalModelMatrix()
	{
		MUSE_PROFILE_FUNCTION();

		if (m_DirtyModel)
		{
			m_LocalModelMatrix = GetLocalTranslationMatrix() * GetLocalRotationMatrix() * GetLocalScaleMatrix();

			m_DirtyModel = false;
		}

		return m_LocalModelMatrix;
	}

    glm::mat4 TransformComponent::GetWorldTranslationMatrix()
    {
		if (m_Parent == nullptr)
		{
			return GetLocalTranslationMatrix();
		}

		return m_Parent->TransformMatrix(GetLocalTranslationMatrix());
    }

    glm::mat4 TransformComponent::GetWorldRotationMatrix()
    {
		if(m_Parent == nullptr)
		{
			return GetLocalRotationMatrix();
		}

		return m_Parent->TransformMatrix(GetLocalRotationMatrix());
    }

    glm::mat4 TransformComponent::GetWorldScaleMatrix()
    {
		if (m_Parent == nullptr)
		{
			return GetLocalScaleMatrix();
		}

		return m_Parent->TransformMatrix(GetLocalScaleMatrix());
    }

    glm::mat4 TransformComponent::GetWorldModelMatrix()
    {
		if (m_Parent == nullptr)
		{
			return GetLocalModelMatrix();
		}

		return m_Parent->TransformMatrix(GetLocalModelMatrix());
    }

    void TransformComponent::AddChild(const std::shared_ptr<TransformComponent>& a_ChildTransformComponent)
    {
		m_Children.push_back(a_ChildTransformComponent);
		a_ChildTransformComponent->SetParent(shared_from_this());
    }

    void TransformComponent::SetParent(const std::shared_ptr<TransformComponent>& a_ParentTransformComponent)
    {
		m_Parent = a_ParentTransformComponent;
    }

    glm::vec3 TransformComponent::InverseTransformPoint(const glm::vec3& a_WorldPoint)
	{
		MUSE_PROFILE_FUNCTION();

		const glm::vec4 transformed = glm::inverse(GetLocalModelMatrix()) * glm::vec4(a_WorldPoint, 1);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformComponent::InverseTransformVector(const glm::vec3& a_WorldVector)
	{
		MUSE_PROFILE_FUNCTION();

		const glm::vec4 transformed = glm::inverse(GetLocalModelMatrix()) * glm::vec4(a_WorldVector, 0);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformComponent::TransformPoint(const glm::vec3& a_LocalPoint)
	{
		MUSE_PROFILE_FUNCTION();

		const glm::vec4 transformed = GetLocalModelMatrix() * glm::vec4(a_LocalPoint, 1);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformComponent::TransformVector(const glm::vec3& a_LocalVector)
	{
		MUSE_PROFILE_FUNCTION();

		const glm::vec4 transformed = GetLocalModelMatrix() * glm::vec4(a_LocalVector, 0);
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