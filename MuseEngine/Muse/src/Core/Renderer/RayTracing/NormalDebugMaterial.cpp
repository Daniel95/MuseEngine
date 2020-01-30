#include "MusePCH.h"

#include "NormalDebugMaterial.h"
#include "SceneObject.h"
#include "Shape.h"
#include "RayHitData.h"
#include "Camera.h"
#include "MaterialHelper.h"
#include "LightSource.h"
#include "AmbientLightSource.h"

NormalDebugMaterial::NormalDebugMaterial(const glm::vec3 & color)
	: Material(color)
{}

NormalDebugMaterial::~NormalDebugMaterial() {}

glm::vec3 NormalDebugMaterial::GetColor(const SceneObject & sceneObject, const glm::vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const
{
	getColorParameters;
	const glm::vec3 normal = (sceneObject.GetNormal(point) + glm::vec3(1, 1, 1)) / 2;
	const glm::vec3 normalDebugColor = glm::vec3(normal.x * color.r, normal.y * color.g, normal.z * color.b);
	return normalDebugColor;
}
