#include "MusePCH.h"

#include "CheckerBoardMaterial.h"
#include "SceneObject.h"
#include "Shape.h"
#include "Scene.h"
#include "RayHitData.h"
#include "Camera.h"
#include "SpeculairMaterial.h"
#include "DiffuseMaterial.h"
#include "Scene.h"
#include "AmbientLightSource.h"
#include "Transform.h"

CheckerBoardMaterial::CheckerBoardMaterial(const sf::Color & color1, const sf::Color & color2, float gridSize, float speculairStrength)
	: BlinnPhongMaterial(color1, speculairStrength), color2(color2), gridSize(gridSize) { }

CheckerBoardMaterial::~CheckerBoardMaterial() {}

sf::Color CheckerBoardMaterial::GetColor(const SceneObject & sceneObject, const vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const
{
	const sf::Color speculair = speculairMaterial.GetSpeculair(sceneObject, point, getColorParameters->RayDirection);
	const sf::Color diffuse = diffuseMaterial.GetDiffuse(sceneObject, point);
	const sf::Color combinedLights = speculair + diffuse + sceneObject.GetScene().GetAmbientLight();

	int gridPositionX = static_cast<int>(round(point.m_X / gridSize));
	int gridPositionY = static_cast<int>(round(point.m_Y / gridSize));
	int gridPositionZ = static_cast<int>(round(point.m_Z / gridSize));

	int positionInGrid = gridPositionX + gridPositionY + gridPositionZ;

	sf::Color result;

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