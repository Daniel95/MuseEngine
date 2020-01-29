#pragma once

#include "Material.h"
#include "BlinnPhongMaterial.h"

class SceneObject;

class ReflectiveMaterial : public BlinnPhongMaterial
{
public:
	ReflectiveMaterial(const sf::Color & color, float speculairStrength = 0.5f, float reflectiveness = 0.5f, int maxBounces = 32);
	~ReflectiveMaterial();

	virtual sf::Color GetColor(const SceneObject & sceneObject, const vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const override;
	sf::Color GetReflection(const SceneObject & sceneObject, const vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const;

private:
	int maxBounces;
	float reflectiveness;

};