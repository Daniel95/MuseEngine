#include "MusePCH.h"

#include "BlinnPhongMaterial.h"
#include "SceneObject.h"
#include "Shape.h"
#include "Scene.h"
#include "RayHitData.h"
#include "Camera.h"
#include "SpeculairMaterial.h"
#include "DiffuseMaterial.h"
#include "Scene.h"
#include "AmbientLightSource.h"

BlinnPhongMaterial::BlinnPhongMaterial(const sf::Color & color, float speculairStrength)
	: Material(color), speculairMaterial(*new SpeculairMaterial(color, speculairStrength)), diffuseMaterial(*new DiffuseMaterial(color)) { }

BlinnPhongMaterial::~BlinnPhongMaterial() { }

sf::Color BlinnPhongMaterial::GetColor(const SceneObject & sceneObject, const vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const
{
	const sf::Color speculair = speculairMaterial.GetSpeculair(sceneObject, point, getColorParameters->RayDirection);
	const sf::Color diffuse = diffuseMaterial.GetDiffuse(sceneObject, point);
	const sf::Color combinedLights = speculair + diffuse + sceneObject.GetScene().GetAmbientLight();
	const sf::Color result = color * combinedLights;

	return result;
}