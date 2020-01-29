#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "MathLibrary.h"
#include "vec3.h"

class SceneObject;
class LightSource;

sf::Color operator*(const sf::Color & a_Lhs, float a_Rhs);
void FilterBlockedLights(std::vector<LightSource*>& lights, const SceneObject & sceneObject, const vec3 & point);
