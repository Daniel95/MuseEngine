#pragma once

#include "Shape.h"

namespace Muse
{
	class SceneObject;

	class Sphere : public Shape
	{
	public:
		Sphere() = default;
		virtual ~Sphere() = default;
		virtual bool CheckRayHit(glm::vec3& a_IntersectionPoint, const Ray& a_Ray) const override;
		virtual glm::vec3 GetNormal(const glm::vec3& a_Point) const override;
		virtual void GetMinMaxBounds(glm::vec3& a_Min, glm::vec3& a_Max) const;
		float GetRadius() const { return m_Radius; }
		void SetRadius(float a_Radius) { m_Radius = a_Radius; }

	private:
		float m_Radius = 1;

		glm::vec3 testPosition;

	};
}