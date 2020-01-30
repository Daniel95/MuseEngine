#pragma once

#include "Material.h"
#include "BlinnPhongMaterial.h"

namespace Muse
{
	class GameObject;

	class ReflectiveMaterial : public BlinnPhongMaterial
	{
	public:
		ReflectiveMaterial(const glm::vec3& color, float speculairStrength = 0.5f, float reflectiveness = 0.5f, int maxBounces = 32);
		~ReflectiveMaterial();

		virtual const glm::vec3 & GetColor(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const override;
		const glm::vec3 & GetReflection(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const;

	private:
		int maxBounces;
		float reflectiveness;

	};
}