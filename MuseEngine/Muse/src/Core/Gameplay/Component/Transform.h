#pragma once
#include "Component.h"

#include <glm/glm.hpp>
//#include "glm/ext/matrix_transform.inl"
//#include "glm/detail/type_quat.hpp"

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

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& a_Position);
		void SetPosition(const glm::vec2& a_Position);
		void Move(const glm::vec3& a_Movement);
		void Move(const glm::vec2& a_Movement);

		const glm::vec3& GetScale() const { return m_Scale; }
		void SetScale(const glm::vec3& a_Scale);
		void SetScale(const glm::vec2& a_Scale);

		//const glm::quat & GetRotation() const { return m_RotationQuaternion; }
		void SetRotation(const glm::quat& a_Rotation);

		const glm::vec4 GetTranslation() const { return glm::vec4(m_Position.x, m_Position.y, m_Position.z, 1); }
		const glm::mat4& GetTranslationMatrix() const { return m_TranslationMatrix; }
		const glm::mat4& GetRotationMatrix() const { return m_RotationMatrix; }
		const glm::mat4& GetScaleMatrix() const { return m_ScaleMatrix; }
		const glm::mat4& GetModelMatrix() const { return m_ModelMatrix; }

		bool IsDirty() const { return !m_DirtyPosition || !m_DirtyScale || !m_DirtyRotation; }

		const glm::mat4& GetTranslationMatrix();
		const glm::mat4& GetRotationMatrix();
		const glm::mat4& GetScaleMatrix();
		const glm::mat4& GetModelMatrix();


		const glm::vec3& RTTRGetPosition() const { return m_Position; }
		void RTTRSetPosition(const glm::vec3& a_Position) { SetPosition(a_Position); }
		const glm::vec3& RTTRGetScale() const { return m_Scale; }
		void RTTRSetScale(const glm::vec3& a_Scale) { SetScale(a_Scale); }

		RTTR_REGISTRATION_FRIEND;

	private:
		bool m_DirtyPosition = true;
		bool m_DirtyRotation = true;
		bool m_DirtyScale = true;


		glm::vec3 m_Position = glm::vec3(0, 0, 0);
		glm::vec3 m_Scale = glm::vec3(1, 1, 1);
		//glm::quat m_RotationQuaternion;

		glm::mat4 m_TranslationMatrix;
		glm::mat4 m_RotationMatrix;
		glm::mat4 m_ScaleMatrix;
		glm::mat4 m_ModelMatrix;


		/*
		glm::vec3 m_Position = glm::vec3(0, 0, 0);
		glm::vec3 m_Scale = glm::vec3(1, 1, 1);
		glm::quat m_RotationQuaternion = glm::identity<glm::quat>();

		glm::mat4 m_TranslationMatrix = glm::identity<glm::mat4>();
		glm::mat4 m_RotationMatrix = glm::identity<glm::mat4>();
		glm::mat4 m_ScaleMatrix = glm::identity<glm::mat4>();
		glm::mat4 m_ModelMatrix = glm::identity<glm::mat4>();
	    */
	};
}
