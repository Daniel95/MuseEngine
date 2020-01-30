#pragma once

#include "Material.h"

#include <glm/glm.hpp>

class SceneObject;

class NormalDebugMaterial : public Material
{
public:
	NormalDebugMaterial(const glm::vec3 & color = glm::vec3(1.f, 1.f, 1.f));
	~NormalDebugMaterial();

	virtual glm::vec3 GetColor(const SceneObject & sceneObject, const glm::vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const override;

};