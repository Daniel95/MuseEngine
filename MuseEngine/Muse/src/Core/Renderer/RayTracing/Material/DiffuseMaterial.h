#pragma once

#include <glm/glm.hpp>

#include "Material.h"


namespace Muse
{

	class GameObject;

	class DiffuseMaterial : public Material
	{
	public:
		DiffuseMaterial(const glm::vec3& color);
		~DiffuseMaterial();

		virtual glm::vec3 GetColor(const GameObject& a_GameObject, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const override;
		virtual glm::vec3 GetDiffuse(const GameObject& a_GameObject, const glm::vec3& point) const;
	};

}