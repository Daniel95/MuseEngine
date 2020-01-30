#pragma once

#include "Material.h"
#include "BlinnPhongMaterial.h"

class SceneObject;

class CheckerBoardMaterial : public BlinnPhongMaterial
{
public:
	CheckerBoardMaterial(const glm::vec3 & color1 = glm::vec3(0.f, 0.f, 1.f), const glm::vec3 & color2 = glm::vec3(1.f, 0.f, 0.f), float gridSize = 5, float speculairStrength = 0.5f);
	~CheckerBoardMaterial();
	virtual glm::vec3 GetColor(const SceneObject & sceneObject, const glm::vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const override;

protected:
	glm::vec3 color2;
	float gridSize;

};

