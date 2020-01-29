#pragma once

#include <SFML/Graphics.hpp>

#include "MathLibrary.h"
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

	virtual sf::Color GetColor(const vec3 & point, std::shared_ptr<GetColorParameters> getColorParameters) const;
	virtual std::unique_ptr<RayHitData> CheckRayHit(const std::shared_ptr<Ray> ray) const;
	virtual vec3 GetNormal(const vec3 & point) const;

protected:
	const Material & ObjectMaterial;
	Scene & ObjectScene;
	Shape & ObjectShape;

};

