#pragma once

#include "Material.h"

class SceneObject;

class NormalDebugMaterial : public Material
{
public:
	NormalDebugMaterial(const sf::Color & color = sf::Color::White);
	~NormalDebugMaterial();

	virtual sf::Color GetColor(const SceneObject & sceneObject, const vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const override;

};