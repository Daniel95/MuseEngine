#include "MusePCH.h"

#include "Box.h"
#include "Shape.h"
#include "Core/Gameplay/Component/TransformComponent.h"
#include "Core/Renderer/RayTracing/Ray.h"

namespace Muse
{
	Box::Box()
	{
		bounds[0] = glm::vec3(-0.5f, -0.5f, -0.5f);
		bounds[1] = glm::vec3(0.5f, 0.5f, 0.5f);
	}

    float Box::CheckRayHit(const Ray& a_Ray) const
    {
		/*
		__m128 t1 = _mm_mul_ps(_mm_sub_ps(node->bmin4, O4), rD4);
		__m128 t2 = _mm_mul_ps(_mm_sub_ps(node->bmax4, O4), rD4);
		__m128 vmax4 = _mm_max_ps(t1, t2), vmin4 = _mm_min_ps(t1, t2);
		float* vmax = (float*)&vmax4, * vmin = (float*)&vmin4;
		float tmax = min(vmax[0], min(vmax[1], vmax[2]));
		float tmin = max(vmin[0], max(vmin[1], vmin[2]));
		return tmax >= tmin && tmax >= 0;
	    */
		return -1;
    }

	/*
bool intersection( box b, ray r )
{
   __m128 t1 = _mm_mul_ps( _mm_sub_ps( node->bmin4, O4 ), rD4 );
   __m128 t2 = _mm_mul_ps( _mm_sub_ps( node->bmax4, O4 ), rD4 );
   __m128 vmax4 = _mm_max_ps( t1, t2 ), vmin4 = _mm_min_ps( t1, t2 );
   float* vmax = (float*)&vmax4, *vmin = (float*)&vmin4;
   float tmax = min(vmax[0], min(vmax[1], vmax[2]));
   float tmin = max(vmin[0], max(vmin[1], vmin[2]));
   return tmax >= tmin && tmax >= 0;
}
		*/

	glm::vec3 Box::GetNormal(const glm::vec3& a_Point) const
	{
		const glm::vec3 offsetToPoint = GetTransform()->InverseTransformPoint(a_Point);
		glm::vec3 normal;

		if (abs(offsetToPoint.x) >= abs(offsetToPoint.y) &&
			abs(offsetToPoint.x) >= abs(offsetToPoint.z))
		{
			normal = glm::vec3(copysignf(1, offsetToPoint.x), 0, 0);
		}
		else if (abs(offsetToPoint.y) >= abs(offsetToPoint.x) &&
			abs(offsetToPoint.y) >= abs(offsetToPoint.z))
		{
			normal = glm::vec3(0, copysignf(1, offsetToPoint.y), 0);
		}
		else
		{
			normal = glm::vec3(0, 0, copysignf(1, offsetToPoint.z));
		}

		return GetTransform()->TransformVector(normal);
	}

	void Box::GetMinMaxBounds(glm::vec3& a_Min, glm::vec3& a_Max) const
	{
		glm::vec3 worldExtends = GetTransform()->TransformVector(glm::vec3(0.5f, 0.5f, 0.5f));
		glm::vec3 shapeMin = GetTransform()->GetPosition() - worldExtends;
		glm::vec3 shapeMax = GetTransform()->GetPosition() + worldExtends;

		a_Min.x = std::min(shapeMin.x, shapeMax.x);
		a_Min.y = std::min(shapeMin.y, shapeMax.y);
		a_Min.z = std::min(shapeMin.z, shapeMax.z);

		a_Max.x = std::max(shapeMin.x, shapeMax.x);
		a_Max.y = std::max(shapeMin.y, shapeMax.y);
		a_Max.z = std::max(shapeMin.z, shapeMax.z);
	}

}
