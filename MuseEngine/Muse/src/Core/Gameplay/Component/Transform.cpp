#include "MusePCH.h"

#include "Core/Gameplay/Component/Transform.h"

namespace Muse
{
	Transform::Transform()
	{
	}

	Transform::~Transform()
	{
	}

    void Transform::SetPosition(const glm::vec3& a_Position)
    {
		m_DirtyPosition = true;
		m_Position = a_Position;
    }

    void Transform::SetPosition(const glm::vec2& a_Position)
    {
		m_DirtyPosition = true;
		m_Position = glm::vec3(a_Position.x, a_Position.y, m_Position.z);
    }

    void Transform::Move(const glm::vec3& a_Movement)
    {
		m_DirtyPosition = true;
		m_Position += a_Movement;
    }

    void Transform::Move(const glm::vec2& a_Movement)
    {
		m_DirtyPosition = true;
		m_Position += glm::vec3(a_Movement.x, a_Movement.y, 0);
    }

    void Transform::SetScale(const glm::vec3& a_Scale)
    {
		m_DirtyScale = true;
		m_Scale = a_Scale;
    }

    void Transform::SetScale(const glm::vec2& a_Scale)
    {
		m_Scale = glm::vec3(a_Scale.x, a_Scale.y, m_Scale.z);;
    }

    void Transform::SetRotation(const glm::quat& a_Rotation)
    {
		//m_RotationQuaternion = a_Rotation;
    }

	const glm::mat4& Transform::GetTranslationMatrix()
	{
		if (m_DirtyPosition)
		{
			//m_TranslationMatrix = DXS::Matrix::CreateTranslation(-m_Position);
			m_DirtyPosition = false;
		}

		return m_TranslationMatrix;
	}

	const glm::mat4& Transform::GetRotationMatrix()
	{
		if (m_DirtyRotation)
		{
			//m_RotationMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationQuaternion(m_RotationQuaternion));
			m_DirtyRotation = false;
		}

		return m_RotationMatrix;
	}

	const glm::mat4& Transform::GetScaleMatrix()
	{
		if (m_DirtyScale)
		{
			//m_ScaleMatrix = DirectX::XMMatrixScalingFromVector(m_Scale);
			m_DirtyScale = false;
		}

		return m_ScaleMatrix;
	}

	const glm::mat4& Transform::GetModelMatrix()
	{
		if (IsDirty())
		{
			m_ModelMatrix = GetScaleMatrix() * GetRotationMatrix() * GetTranslationMatrix();
		}

		return m_ModelMatrix;
	}

    /*
	const DXS::Vector3 & Transform::GetPosition() const
	{
		return m_Position;
	}

	void Transform::SetPosition(const DXS::Vector3& a_Position)
	{
		m_Position = a_Position;

		m_DirtyPosition = true;
		m_DirtyModel = true;
	}

	void Transform::SetPosition(const DXS::Vector2& a_Position)
	{
		SetPosition(DXS::Vector3(a_Position.x, a_Position.y, m_Position.z));
	}

	void Transform::Move(const DXS::Vector3& a_Movement)
	{
		SetPosition(GetPosition() + a_Movement);
	}

	void Transform::Move(const DXS::Vector2& a_Movement)
	{
		Move(DXS::Vector3(a_Movement.x, a_Movement.y, 0));
	}

	const DXS::Vector3 & Transform::GetScale() const
	{
		return m_Scale;
	}

	void Transform::SetScale(const DXS::Vector3& a_Scale)
	{
		m_Scale = a_Scale;

		m_DirtyScale = true;
		m_DirtyModel = true;
	}

	void Transform::SetScale(const DXS::Vector2& a_Scale)
	{
		SetScale(DXS::Vector3(a_Scale.x, a_Scale.y, m_Scale.z));
	}

	const DXS::Quaternion & Transform::GetRotation() const
	{
		return m_RotationQuaternion;
	}

	void Transform::SetRotation(const DXS::Quaternion& a_Rotation)
	{
		m_RotationQuaternion = a_Rotation;

		m_DirtyRotation = true;
		m_DirtyModel = true;
	}

	const DXS::Vector4 Transform::GetTranslation()
	{
		return DXS::Vector4(m_Position.x, m_Position.y, m_Position.z, 1);
	}

	const DXS::Matrix& Transform::GetTranslationMatrix()
	{
		if (m_DirtyPosition)
		{
			m_TranslationMatrix = DXS::Matrix::CreateTranslation(-m_Position);
			m_DirtyPosition = false;
		}

		return m_TranslationMatrix;
	}

	const DXS::Matrix& Transform::GetRotationMatrix()
	{
		if (m_DirtyRotation)
		{
			m_RotationMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationQuaternion(m_RotationQuaternion));
			m_DirtyRotation = false;
		}

		return m_RotationMatrix;
	}

	const DXS::Matrix& Transform::GetScaleMatrix()
	{
		if (m_DirtyScale)
		{
			m_ScaleMatrix = DirectX::XMMatrixScalingFromVector(m_Scale);
			m_DirtyScale = false;
		}

		return m_ScaleMatrix;
	}

	const DXS::Matrix& Transform::GetModelMatrix()
	{
		if (m_DirtyModel)
		{
			m_ModelMatrix = GetScaleMatrix() * GetRotationMatrix() * GetTranslationMatrix();
			m_DirtyModel = false;
		}

		return m_ModelMatrix;
	}

	const DXS::Vector3& Transform::RTTRGetPosition() const
	{
		return GetPosition();
	}

	void Transform::RTTRSetPosition(const DXS::Vector3& a_Position)
	{
		SetPosition(a_Position);
	}

	const DXS::Vector3& Transform::RTTRGetScale() const
	{
		return GetScale();
	}

	void Transform::RTTRSetScale(const DXS::Vector3& a_Scale)
	{
		SetScale(a_Scale);
	}
    */
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

	rttr::registration::class_<Engine::Transform>("Transform")
		.constructor<>()
		(
			rttr::policy::ctor::as_raw_ptr
		)
		.property("Position", &Engine::Transform::m_Position)
		.property("Scale", &Engine::Transform::m_Scale)
		.property("Rotation", &Engine::Transform::m_RotationQuaternion);

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