#pragma once
#include "Component.h"

#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#undef countof
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"
//#include <glm/gtx/quaternion.hpp>
#undef countof

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
namespace Muse
{
	class GameObject;

	class TransformComponent : public Component, std::enable_shared_from_this<TransformComponent>
	{
		RTTR_ENABLE(Component)
		RTTR_REGISTRATION_FRIEND

	public:
		TransformComponent() = default;
		virtual ~TransformComponent() = default;

		const glm::vec3& GetPosition() const { return m_LocalPosition; }
		void SetPosition(const glm::vec3& a_Position);
		void SetPosition(const glm::vec2& a_Position);
		void Move(const glm::vec3& a_Movement);
		void Move(const glm::vec2& a_Movement);

		const glm::vec3& GetScale() const { return m_LocalScale; }
		void SetScale(const glm::vec3& a_Scale);
		void SetScale(const glm::vec2& a_Scale);

		const glm::vec3 & GetRotation() const { return m_LocalRotation; }
		void SetRotation(const glm::vec3& a_Rotation);
		void Rotate(const glm::vec3& a_Rotation);

		const glm::quat & GetRotationQuat() const { return m_RotationQuaternion; }
		void SetRotationQuat(const glm::quat& a_Rotation);

		glm::vec3 InverseTransformPoint(const glm::vec3& a_WorldPoint); //World to local point 
		glm::vec3 InverseTransformVector(const glm::vec3& a_WorldVector); //World to local vector
		glm::vec3 TransformPoint(const glm::vec3& a_LocalPoint); //Local to world point
		glm::vec3 TransformVector(const glm::vec3& a_LocalVector); //Local to world vector

		glm::mat4 InverseTransformMatrix(const glm::mat4& a_WorldMatrix) { return glm::inverse(GetWorldModelMatrix() * a_WorldMatrix); } //World to local point 
		glm::mat4 TransformMatrix(const glm::mat4& a_LocalMatrix) { return GetWorldModelMatrix() * a_LocalMatrix;  }  //Local to world point

		glm::vec3 GetForward() { return TransformVector(glm::vec3(0, 0, 1)); }
		glm::vec3 GetUp() { return TransformVector(glm::vec3(0, 1, 0));; }
		glm::vec3 GetRight() { return TransformVector(glm::vec3(1, 0, 0)); }

		const glm::mat4& GetLocalTranslationMatrix();
		const glm::mat4& GetLocalRotationMatrix();
		const glm::mat4& GetLocalScaleMatrix();
		const glm::mat4& GetLocalModelMatrix();

		glm::mat4 GetWorldTranslationMatrix();
		glm::mat4 GetWorldRotationMatrix();
		glm::mat4 GetWorldScaleMatrix();
		glm::mat4 GetWorldModelMatrix();

		void AddChild(const std::shared_ptr<TransformComponent>& a_ChildTransformComponent);
		void SetParent(const std::shared_ptr<TransformComponent>& a_ParentTransformComponent);

		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(cereal::make_nvp("Component", cereal::base_class<Component>(this)));
			ar(
				m_LocalPosition,
				m_LocalScale,
				m_LocalRotation,
				m_RotationQuaternion
			);
		}

	private:
		bool m_DirtyPosition = true;
		bool m_DirtyRotation = true;
		bool m_DirtyScale = true;
		bool m_DirtyModel = true;

		glm::vec3 m_LocalPosition = glm::vec3(0, 0, 0);
		glm::vec3 m_LocalScale = glm::vec3(1, 1, 1);
		glm::vec3 m_LocalRotation = glm::vec3(0, 0, 0);
		glm::quat m_RotationQuaternion = glm::identity<glm::quat>();

		glm::mat4 m_LocalTranslationMatrix = glm::identity<glm::mat4>();
		glm::mat4 m_LocalRotationMatrix = glm::identity<glm::mat4>();
		glm::mat4 m_LocalScaleMatrix = glm::identity<glm::mat4>();
		glm::mat4 m_LocalModelMatrix = glm::identity<glm::mat4>();

		std::vector<std::shared_ptr<TransformComponent>> m_Children;
		std::shared_ptr<TransformComponent> m_Parent = nullptr;
	};
}

CEREAL_REGISTER_TYPE_WITH_NAME(Muse::TransformComponent, "TransformComponent")
CEREAL_REGISTER_POLYMORPHIC_RELATION(Muse::Component, Muse::TransformComponent)

namespace glm
{

	template<class Archive> void serialize(Archive& archive, glm::vec2& v) { archive(v.x, v.y); }
	template<class Archive> void serialize(Archive& archive, glm::vec3& v) { archive(v.x, v.y, v.z); }
	template<class Archive> void serialize(Archive& archive, glm::vec4& v) { archive(v.x, v.y, v.z, v.w); }
	template<class Archive> void serialize(Archive& archive, glm::ivec2& v) { archive(v.x, v.y); }
	template<class Archive> void serialize(Archive& archive, glm::ivec3& v) { archive(v.x, v.y, v.z); }
	template<class Archive> void serialize(Archive& archive, glm::ivec4& v) { archive(v.x, v.y, v.z, v.w); }
	template<class Archive> void serialize(Archive& archive, glm::uvec2& v) { archive(v.x, v.y); }
	template<class Archive> void serialize(Archive& archive, glm::uvec3& v) { archive(v.x, v.y, v.z); }
	template<class Archive> void serialize(Archive& archive, glm::uvec4& v) { archive(v.x, v.y, v.z, v.w); }
	template<class Archive> void serialize(Archive& archive, glm::dvec2& v) { archive(v.x, v.y); }
	template<class Archive> void serialize(Archive& archive, glm::dvec3& v) { archive(v.x, v.y, v.z); }
	template<class Archive> void serialize(Archive& archive, glm::dvec4& v) { archive(v.x, v.y, v.z, v.w); }

	// glm matrices serialization
	template<class Archive> void serialize(Archive& archive, glm::mat2& m) { archive(m[0], m[1]); }
	template<class Archive> void serialize(Archive& archive, glm::dmat2& m) { archive(m[0], m[1]); }
	template<class Archive> void serialize(Archive& archive, glm::mat3& m) { archive(m[0], m[1], m[2]); }
	template<class Archive> void serialize(Archive& archive, glm::mat4& m) { archive(m[0], m[1], m[2], m[3]); }
	template<class Archive> void serialize(Archive& archive, glm::dmat4& m) { archive(m[0], m[1], m[2], m[3]); }

	template<class Archive> void serialize(Archive& archive, glm::quat& q) { archive(q.x, q.y, q.z, q.w); }
	template<class Archive> void serialize(Archive& archive, glm::dquat& q) { archive(q.x, q.y, q.z, q.w); }

}