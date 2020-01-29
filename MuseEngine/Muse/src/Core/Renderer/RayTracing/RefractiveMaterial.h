#pragma once

#include "Material.h"
#include "BlinnPhongMaterial.h"

class SceneObject;

class RefractiveMaterial : public BlinnPhongMaterial
{
public:
	RefractiveMaterial(const sf::Color & color, float speculairStrength = 0.5f, float refractiveness = 0.5f, float eta = 0.975f, int maxBounces = 32);
	~RefractiveMaterial();

	virtual sf::Color GetColor(const SceneObject & sceneObject, const vec3 & point, std::shared_ptr<GetColorParameters>getColorParameters) const override;
	sf::Color GetRefraction(const SceneObject & sceneObject, const vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const;

private:
	int maxBounces;
	float refractiveness;
	float eta;

};