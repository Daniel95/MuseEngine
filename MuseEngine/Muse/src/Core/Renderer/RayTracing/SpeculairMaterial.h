#pragma once

#include "Material.h"

#include <glm/glm.hpp>

class SceneObject;

class SpeculairMaterial : public Material
{
public:
	SpeculairMaterial(glm::vec3 color, float speculairStrength = 0.5f);
	~SpeculairMaterial();

	virtual glm::vec3 GetColor(const SceneObject & sceneObject, const glm::vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const override;
	const glm::vec3 GetSpeculair(const SceneObject & sceneObject, const glm::vec3 & point, const glm::vec3 & lookDirection) const;

private:
	float speculairStrength;

};