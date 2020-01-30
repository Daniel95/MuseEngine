#pragma once

#include "Material.h"

class SceneObject;
class DiffuseMaterial;
class SpeculairMaterial;

class BlinnPhongMaterial : public Material
{
public:
	BlinnPhongMaterial(const glm::vec3 & color, float speculairStrength = 0.5f);
	~BlinnPhongMaterial();
	virtual glm::vec3 GetColor(const SceneObject & sceneObject, const glm::vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const override;

protected:
	DiffuseMaterial& diffuseMaterial;
	SpeculairMaterial& speculairMaterial;

};

