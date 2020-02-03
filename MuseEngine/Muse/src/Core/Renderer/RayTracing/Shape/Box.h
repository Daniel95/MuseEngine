#pragma once

#include "Shape.h"
#include "Plane.h"

namespace Muse
{
	class Box : public Shape
	{
	public:
		Box();
		virtual ~Box() = default;
		virtual bool CheckRayHit(glm::vec3& a_IntersectionPoint, const Ray& a_Ray) const override;
		virtual glm::vec3 GetNormal(const glm::vec3& a_Point) const override;
		virtual void GetMinMaxBounds(glm::vec3& a_Min, glm::vec3& a_Max) const;

	private:
		glm::vec3 bounds[2];
	};

}