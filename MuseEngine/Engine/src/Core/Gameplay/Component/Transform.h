#pragma once
#include "Component.h"

#include <rttr/registration_friend>

namespace Muse
{
	class GameObject;
	struct DX12Matrix;

	class Transform : public Component
	{
        RTTR_ENABLE(Component);

	public:
		Transform();
		virtual ~Transform();

        /*
		const DXS::Vector3 & GetPosition() const;
		void SetPosition(const DXS::Vector3& a_Position);
		void SetPosition(const DXS::Vector2& a_Position);
		void Move(const DXS::Vector3& a_Movement);
		void Move(const DXS::Vector2& a_Movement);

		const DXS::Vector3& GetScale() const;
		void SetScale(const DXS::Vector3& a_Scale);
		void SetScale(const DXS::Vector2& a_Scale);

		const DXS::Quaternion & GetRotation() const;
		void SetRotation(const DXS::Quaternion& a_Rotation);

		const DXS::Vector4 GetTranslation();
		const DXS::Matrix& GetTranslationMatrix();
		const DXS::Matrix& GetRotationMatrix();
		const DXS::Matrix& GetScaleMatrix();
		const DXS::Matrix& GetModelMatrix();

		const bool IsDirty() const;

		const DXS::Vector3& RTTRGetPosition() const;
		void RTTRSetPosition(const DXS::Vector3& a_Position);
		const DXS::Vector3& RTTRGetScale() const;
		void RTTRSetScale(const DXS::Vector3& a_Scale);

		RTTR_REGISTRATION_FRIEND;

	private:
		bool m_DirtyPosition = true;
		bool m_DirtyRotation = true;
		bool m_DirtyScale = true;
		bool m_DirtyModel = true;

		DXS::Vector3 m_Position = DXS::Vector3::Zero;
		DXS::Vector3 m_Scale = DXS::Vector3::One;
		DXS::Quaternion m_RotationQuaternion = DXS::Quaternion::Identity;

		DXS::Matrix m_TranslationMatrix = DXS::Matrix::Identity;
		DXS::Matrix m_RotationMatrix = DXS::Matrix::Identity;
		DXS::Matrix m_ScaleMatrix = DXS::Matrix::Identity;
		DXS::Matrix m_ModelMatrix = DXS::Matrix::Identity;
        */
	};
}