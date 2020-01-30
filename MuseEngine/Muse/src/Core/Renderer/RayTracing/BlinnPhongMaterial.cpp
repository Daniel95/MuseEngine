#include "MusePCH.h"

#include "BlinnPhongMaterial.h"
#include "SceneObject.h"
#include "Shape.h"
#include "RayHitData.h"
#include "Camera.h"
#include "SpeculairMaterial.h"
#include "DiffuseMaterial.h"
#include "AmbientLightSource.h"
#include "Core/Scene/Scene.h"

BlinnPhongMaterial::BlinnPhongMaterial(const glm::vec3 & color, float speculairStrength)
	: Material(color), speculairMaterial(*new SpeculairMaterial(color, speculairStrength)), diffuseMaterial(*new DiffuseMaterial(color)) { }

BlinnPhongMaterial::~BlinnPhongMaterial() { }

glm::vec3 BlinnPhongMaterial::GetColor(const Muse::GameObject & a_gameObject, const glm::vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const
{
	const glm::vec3 speculair = speculairMaterial.GetSpeculair(a_gameObject, point, getColorParameters->RayDirection);
	const glm::vec3 diffuse = diffuseMaterial.GetDiffuse(a_gameObject, point);
	const glm::vec3 combinedLights = speculair + diffuse + 0.1f;//a_gameObject.GetScene().GetAmbientLight();
	const glm::vec3 result = color * combinedLights;

	return result;
}