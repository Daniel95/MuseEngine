#include "MusePCH.h"

#include "Core/Gameplay/Component/TransformComponentOld.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/transform.hpp>
#include "Core/Instrumentor.h"

namespace Muse
{
    glm::vec3 TransformComponentOld::GetWorldPosition() const
    {
		if (!HasParent())
		{
			return m_LocalPosition;
		}

		return m_Parent->TransformPoint(m_LocalPosition);
    }

	glm::vec3 TransformComponentOld::GetWorldScale() const
	{
		if (!HasParent())
		{
			return m_LocalScale;
		}

		return m_Parent->TransformVector(m_LocalScale);
	}

    void TransformComponentOld::SetLocalPosition(const glm::vec3& a_Position)
    {
		MUSE_PROFILE_FUNCTION();

		//m_DirtyPosition = m_DirtyModel = true;

		SetDirty();
		m_LocalPosition = a_Position;
    }

    void TransformComponentOld::SetLocalPosition(const glm::vec2& a_Position)
    {
		MUSE_PROFILE_FUNCTION();

		//m_DirtyPosition = m_DirtyModel = true;

		SetDirty();
		m_LocalPosition = glm::vec3(a_Position.x, a_Position.y, m_LocalPosition.z);
    }

    void TransformComponentOld::Translate(const glm::vec3& a_Movement)
    {
		MUSE_PROFILE_FUNCTION();

		//m_DirtyPosition = m_DirtyModel = true;
    	
		SetDirty();
		m_LocalPosition += a_Movement;
    }

    void TransformComponentOld::Translate(const glm::vec2& a_Movement)
    {
		MUSE_PROFILE_FUNCTION();

		//m_DirtyPosition = m_DirtyModel = true;
    	
		SetDirty();
		m_LocalPosition += glm::vec3(a_Movement.x, a_Movement.y, 0);
    }

    void TransformComponentOld::SetLocalScale(const glm::vec3& a_Scale)
    {
		MUSE_PROFILE_FUNCTION();

		//m_DirtyScale = m_DirtyModel = true;
    	
		SetDirty();
		m_LocalScale = a_Scale;
    }

    void TransformComponentOld::SetLocalScale(const glm::vec2& a_Scale)
    {
		MUSE_PROFILE_FUNCTION();

		//m_DirtyScale = m_DirtyModel = true;
    	
		SetDirty();
		m_LocalScale = glm::vec3(a_Scale.x, a_Scale.y, m_LocalScale.z);
    }

	void TransformComponentOld::ScaleLocal(const glm::vec2& a_Scale)
	{
		SetDirty();
		m_LocalScale += glm::vec3(a_Scale.x, a_Scale.y, 0);
	}

	void TransformComponentOld::ScaleLocal(const glm::vec3& a_Scale)
	{
		SetDirty();
		m_LocalScale += a_Scale;
	}

	void TransformComponentOld::SetLocalRotation(const glm::vec3& a_Rotation)
    {
		MUSE_PROFILE_FUNCTION();

		//m_DirtyRotation = m_DirtyModel = true;
    	
		SetDirty();
		m_LocalRotation = a_Rotation;
    }

	void TransformComponentOld::RotateLocal(const glm::vec3& a_Rotation)
    {
		MUSE_PROFILE_FUNCTION();

		//m_DirtyRotation = m_DirtyModel = true;
    	
		SetDirty();
		m_LocalRotation += a_Rotation;
    }

	const glm::mat4& TransformComponentOld::GetWorldModelMatrix()
	{
		MUSE_PROFILE_FUNCTION();

		if (m_Dirty)
		{
			m_WorldModelMatrix = CalculateWorldTranslationMatrix() * CalculateWorldRotationMatrix() * CalculateWorldScaleMatrix();
			m_Dirty = false;
		}

		return m_WorldModelMatrix;
	}

    glm::mat4 TransformComponentOld::CalculateWorldRotationMatrix() const
    {
		glm::mat4 localRotationMatrix = glm::rotate(m_LocalRotation.x, glm::vec3(1, 0, 0));
		localRotationMatrix = glm::rotate(localRotationMatrix, m_LocalRotation.y, glm::vec3(0, 1, 0));
		localRotationMatrix = glm::rotate(localRotationMatrix, m_LocalRotation.z, glm::vec3(0, 0, 1));

		if (!HasParent())
		{
			return localRotationMatrix;
		}

		return m_Parent->CalculateWorldRotationMatrix() * localRotationMatrix;
    }

    void TransformComponentOld::AddChild(const std::shared_ptr<TransformComponentOld>& a_ChildTransformComponent)
    {
		if(a_ChildTransformComponent->HasParent())
		{
			a_ChildTransformComponent->GetParent()->RemoveChild(a_ChildTransformComponent);
		}

		m_Children.push_back(a_ChildTransformComponent);
		a_ChildTransformComponent->SetParent(shared_from_this());
    }

    void TransformComponentOld::RemoveChild(const std::shared_ptr<TransformComponentOld>& a_ChildTransformComponent)
	{
		ASSERT_ENGINE(std::find(m_Children.begin(), m_Children.end(), a_ChildTransformComponent) != m_Children.end(), "Transform to remove as a child is not a child!")

		m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), a_ChildTransformComponent), m_Children.end());
		a_ChildTransformComponent->SetParent(nullptr);
	}

    void TransformComponentOld::SetParent(const std::shared_ptr<TransformComponentOld>& a_ParentTransformComponent)
    {
		m_Parent = a_ParentTransformComponent;
    }

    glm::vec3 TransformComponentOld::InverseTransformPoint(const glm::vec3& a_WorldPoint)
	{
		MUSE_PROFILE_FUNCTION();

		const glm::vec4 transformed = glm::inverse(GetWorldModelMatrix()) * glm::vec4(a_WorldPoint, 1);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformComponentOld::InverseTransformVector(const glm::vec3& a_WorldVector)
	{
		MUSE_PROFILE_FUNCTION();

		const glm::vec4 transformed = glm::inverse(GetWorldModelMatrix()) * glm::vec4(a_WorldVector, 0);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformComponentOld::TransformPoint(const glm::vec3& a_LocalPoint)
	{
		MUSE_PROFILE_FUNCTION();

		const glm::vec4 transformed = GetWorldModelMatrix() * glm::vec4(a_LocalPoint, 1);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformComponentOld::TransformVector(const glm::vec3& a_LocalVector)
	{
		MUSE_PROFILE_FUNCTION();

		const glm::vec4 transformed = GetWorldModelMatrix() * glm::vec4(a_LocalVector, 0);
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