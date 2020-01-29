#pragma once

#include "Material.h"
#include "BlinnPhongMaterial.h"

class SceneObject;

class CheckerBoardMaterial : public BlinnPhongMaterial
{
public:
	CheckerBoardMaterial(const sf::Color & color1 = sf::Color::Yellow, const sf::Color & color2 = sf::Color::Red, float gridSize = 5, float speculairStrength = 0.5f);
	~CheckerBoardMaterial();
	virtual sf::Color GetColor(const SceneObject & sceneObject, const vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const override;

protected:
	sf::Color color2;
	float gridSize;

};

