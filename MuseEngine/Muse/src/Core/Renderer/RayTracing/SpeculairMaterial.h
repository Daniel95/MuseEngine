#pragma once

#include "Material.h"

class SceneObject;

class SpeculairMaterial : public Material
{
public:
	SpeculairMaterial(sf::Color color, float speculairStrength = 0.5f);
	~SpeculairMaterial();

	virtual sf::Color GetColor(const SceneObject & sceneObject, const vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const override;
	const sf::Color GetSpeculair(const SceneObject & sceneObject, const vec3 & point, const vec3 & lookDirection) const;

private:
	float speculairStrength;

};