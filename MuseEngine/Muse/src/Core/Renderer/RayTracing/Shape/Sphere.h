#pragma once

#include "Shape.h"

#include <glm/glm.hpp>


namespace Muse
{

	class SceneObject;

	class Sphere : public Shape
	{
	public:
		Sphere(float a_Radius);
		virtual ~Sphere() = default;
		virtual bool CheckRayHit(glm::vec3& a_IntersectionPoint, const std::shared_ptr<Ray> a_Ray) const override;
		virtual glm::vec3 GetNormal(const glm::vec3& a_Point) const override;
		virtual void GetMinMaxBounds(glm::vec3& a_Min, glm::vec3& a_Max) const;

	private:
		float radius;

	};
}