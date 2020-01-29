#pragma once

#include <SFML/Graphics.hpp>
#include "MathLibrary.h"
#include "GetColorParameters.h"
#include "vec3.h"

class SceneObject;

class Material
{
public:
	Material(sf::Color color);
	virtual ~Material();

	const sf::Color & GetColor() const;
	virtual sf::Color GetColor(const SceneObject& sceneObject, const vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const = 0;

protected:
	sf::Color color;

};

