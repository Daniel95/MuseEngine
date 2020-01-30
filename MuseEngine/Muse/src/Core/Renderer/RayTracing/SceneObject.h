#pragma once

#include "Transform.h"
#include "GetColorParameters.h"

class Scene;
class Material;
class Shape;
struct RayHitData;
struct Ray;

class SceneObject
{
public:
	SceneObject(Scene & scene, const Material & material, Shape & shape);
	virtual ~SceneObject();
	const Scene & GetScene() const;
	const Material & GetMaterial() const;
	Shape & GetShape();

	virtual glm::vec3 GetColor(const glm::vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const;
	virtual std::unique_ptr<RayHitData> CheckRayHit(const std::shared_ptr<Ray> ray) const;
	virtual glm::vec3 GetNormal(const glm::vec3 & point) const;

protected:
	const Material & ObjectMaterial;
	Scene & ObjectScene;
	Shape & ObjectShape;

};

