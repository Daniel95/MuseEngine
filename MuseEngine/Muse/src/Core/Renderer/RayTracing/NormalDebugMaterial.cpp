#include "MusePCH.h"

#include "NormalDebugMaterial.h"
#include "SceneObject.h"
#include "Shape.h"
#include "Scene.h"
#include "RayHitData.h"
#include "Camera.h"
#include "MaterialHelper.h"
#include "LightSource.h"
#include "AmbientLightSource.h"

NormalDebugMaterial::NormalDebugMaterial(const sf::Color & color)
	: Material(color)
{}

NormalDebugMaterial::~NormalDebugMaterial() {}

sf::Color NormalDebugMaterial::GetColor(const SceneObject & sceneObject, const vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const
{
	getColorParameters;
	const vec3 normal = (sceneObject.GetNormal(point) + vec3(1, 1, 1)) / 2;
	const sf::Color normalDebugColor = sf::Color(static_cast<sf::Uint8>(normal.m_X * color.r), static_cast<sf::Uint8>(normal.m_Y * color.g), static_cast<sf::Uint8>(normal.m_Z * color.b));
	return normalDebugColor;
}
