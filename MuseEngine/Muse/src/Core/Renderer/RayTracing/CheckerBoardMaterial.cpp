#include "MusePCH.h"

#include "CheckerBoardMaterial.h"
#include "SceneObject.h"
#include "Shape.h"
#include "RayHitData.h"
#include "Camera.h"
#include "SpeculairMaterial.h"
#include "DiffuseMaterial.h"
#include "AmbientLightSource.h"
#include "Transform.h"

CheckerBoardMaterial::CheckerBoardMaterial(const glm::vec3 & color1, const glm::vec3 & color2, float gridSize, float speculairStrength)
	: BlinnPhongMaterial(color1, speculairStrength), color2(color2), gridSize(gridSize) { }

CheckerBoardMaterial::~CheckerBoardMaterial() {}

glm::vec3 CheckerBoardMaterial::GetColor(const SceneObject & sceneObject, const glm::vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const
{
	const glm::vec3 speculair = speculairMaterial.GetSpeculair(sceneObject, point, getColorParameters->RayDirection);
	const glm::vec3 diffuse = diffuseMaterial.GetDiffuse(sceneObject, point);
	const glm::vec3 combinedLights = speculair + diffuse + sceneObject.GetScene().GetAmbientLight();

	int gridPositionX = static_cast<int>(round(point.x / gridSize));
	int gridPositionY = static_cast<int>(round(point.y / gridSize));
	int gridPositionZ = static_cast<int>(round(point.z / gridSize));

	int positionInGrid = gridPositionX + gridPositionY + gridPositionZ;

	glm::vec3 result;

	if (positionInGrid % 2 == 0)
	{
		result = color * combinedLights;
	}
	else
	{
		result = color2 * combinedLights;
	}

	return result;
}