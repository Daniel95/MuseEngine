#pragma once

#include "Material.h"

class SceneObject;
class DiffuseMaterial;
class SpeculairMaterial;

class BlinnPhongMaterial : public Material
{
public:
	BlinnPhongMaterial(const sf::Color & color, float speculairStrength = 0.5f);
	~BlinnPhongMaterial();
	virtual sf::Color GetColor(const SceneObject & sceneObject, const vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const override;

protected:
	DiffuseMaterial& diffuseMaterial;
	SpeculairMaterial& speculairMaterial;

};

