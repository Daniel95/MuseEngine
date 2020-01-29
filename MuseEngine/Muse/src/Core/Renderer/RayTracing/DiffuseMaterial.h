#pragma once

#include "Material.h"
#include "vec3.h"

class SceneObject;

class DiffuseMaterial : public Material
{
public:
	DiffuseMaterial(const sf::Color & color);
	~DiffuseMaterial();

	virtual sf::Color GetColor(const SceneObject & sceneObject, const vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const override;
	virtual sf::Color GetDiffuse(const SceneObject & sceneObject, const vec3 & point) const;
};

