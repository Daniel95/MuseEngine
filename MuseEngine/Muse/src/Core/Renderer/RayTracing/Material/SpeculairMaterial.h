#pragma once

#include "Material.h"

#include <glm/glm.hpp>


namespace Muse
{

	class SceneObject;

	class SpeculairMaterial : public Material
	{
	public:
		SpeculairMaterial(glm::vec3 color, float speculairStrength = 0.5f);
		~SpeculairMaterial();

		virtual glm::vec3 GetColor(const GameObject& sceneObject, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const override;
		const glm::vec3 GetSpeculair(const GameObject& sceneObject, const glm::vec3& point, const glm::vec3& lookDirection) const;

	private:
		float m_SpeculairStrength;
		glm::vec3 m_Color;

	};

}