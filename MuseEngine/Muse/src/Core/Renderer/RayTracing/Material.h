#pragma once

#include "GetColorParameters.h"

class SceneObject;

class Material
{
public:
	Material(glm::vec3 a_Color);
	virtual ~Material();

	const glm::vec3 & GetColor() const;
	virtual glm::vec3 GetColor(const SceneObject& sceneObject, const glm::vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const = 0;

protected:
	glm::vec3 color;

};

