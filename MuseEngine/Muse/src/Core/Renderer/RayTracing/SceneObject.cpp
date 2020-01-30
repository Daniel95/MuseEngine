#include "MusePCH.h"

#include "SceneObject.h"
#include "RayHitData.h"
#include "Material.h"
#include "Shape.h"
#include "Ray.h"

SceneObject::SceneObject(Scene& scene, const Material& material, Shape& shape)
	: ObjectScene(scene), ObjectMaterial(material), ObjectShape(shape) { }

SceneObject::~SceneObject() { }

const Scene& SceneObject::GetScene() const
{
	return ObjectScene;
}

const Material & SceneObject::GetMaterial() const
{
	return ObjectMaterial;
}

Shape & SceneObject::GetShape()
{
	return ObjectShape;
}

glm::vec3 SceneObject::GetColor(const glm::vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const
{
	return ObjectMaterial.GetColor(*this, point, getColorParameters);
}

std::unique_ptr<RayHitData> SceneObject::CheckRayHit(const std::shared_ptr<Ray> ray) const
{
	ObjectScene.IncreaseRayCastsSendThisUpdate();

    glm::vec3 intersectionPoint;

	if (ObjectShape.CheckRayHit(intersectionPoint, ray))
	{
		ObjectScene.IncreaseRayCastsHitThisUpdate();

		return std::make_unique<RayHitData>(this, intersectionPoint);
	}
	else
	{
		return NULL;
	}
}

glm::vec3 SceneObject::GetNormal(const glm::vec3 & point) const
{
	return ObjectShape.GetNormal(point);
}
