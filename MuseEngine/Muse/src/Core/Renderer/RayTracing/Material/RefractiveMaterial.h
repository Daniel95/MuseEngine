#pragma once

#include "Material.h"
#include "BlinnPhongMaterial.h"


namespace Muse
{

	class SceneObject;

	class RefractiveMaterial : public BlinnPhongMaterial
	{
	public:
		RefractiveMaterial(const glm::vec3& color, float speculairStrength = 0.5f, float refractiveness = 0.5f, float eta = 0.975f, int maxBounces = 32);
		~RefractiveMaterial();

		virtual glm::vec3 GetColor(const GameObject& sceneObject, const glm::vec3& point, std::shared_ptr<GetColorParameters>getColorParameters) const override;
		glm::vec3 GetRefraction(const GameObject& sceneObject, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const;

	private:
		int maxBounces;
		float refractiveness;
		float eta;

	};

}