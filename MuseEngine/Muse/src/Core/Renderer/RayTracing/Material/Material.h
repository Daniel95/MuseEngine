#pragma once

#include <glm/glm.hpp>


namespace Muse
{

	struct GetColorParameters;
	class GameObject;

	class Material
	{
	public:
		virtual ~Material() = default;

		virtual glm::vec3 GetColor(const GameObject& a_SceneObject, const glm::vec3& a_Point, std::shared_ptr<GetColorParameters> a_GetColorParameters) const = 0;

	};

}