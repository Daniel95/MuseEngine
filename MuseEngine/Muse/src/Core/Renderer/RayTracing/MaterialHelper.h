#pragma once

#include <vector>

#include <glm/glm.hpp>

class SceneObject;
class LightSource;

glm::vec3 operator*(const glm::vec3 & a_Lhs, float a_Rhs);
void FilterBlockedLights(std::vector<LightSource*>& lights, const SceneObject & sceneObject, const glm::vec3 & point);
