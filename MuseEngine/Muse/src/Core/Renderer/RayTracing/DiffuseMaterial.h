#pragma once

#include <glm/glm.hpp>

#include "Material.h"

class SceneObject;

class DiffuseMaterial : public Material
{
public:
	DiffuseMaterial(const glm::vec3 & color);
	~DiffuseMaterial();

	virtual glm::vec3 GetColor(const SceneObject & sceneObject, const glm::vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const override;
	virtual glm::vec3 GetDiffuse(const SceneObject & sceneObject, const glm::vec3 & point) const;
};

