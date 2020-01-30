#pragma once

#include "Material.h"
#include "BlinnPhongMaterial.h"

class SceneObject;

class ReflectiveMaterial : public BlinnPhongMaterial
{
public:
	ReflectiveMaterial(const glm::vec3 & color, float speculairStrength = 0.5f, float reflectiveness = 0.5f, int maxBounces = 32);
	~ReflectiveMaterial();

	virtual glm::vec3 GetColor(const SceneObject & sceneObject, const glm::vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const override;
	glm::vec3 GetReflection(const SceneObject & sceneObject, const glm::vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const;

private:
	int maxBounces;
	float reflectiveness;

};