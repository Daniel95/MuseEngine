#include "MusePCH.h"

#include "MaterialHelper.h"
#include "LightSource.h"
#include "SceneObject.h"
#include "Ray.h"

glm::vec3 operator*(const glm::vec3 & a_Lhs, float a_Rhs)
{
	return glm::vec3(a_Lhs.r * a_Rhs, a_Lhs.g * a_Rhs, a_Lhs.b * a_Rhs);
}

void FilterBlockedLights(std::vector<LightSource*>& lights, const SceneObject & sceneObject, const glm::vec3 & point)
{
	std::vector<SceneObject*> sceneObjects = sceneObject.GetScene().GetSceneObjects();
	sceneObjects.erase(std::remove(sceneObjects.begin(), sceneObjects.end(), &sceneObject), sceneObjects.end());

	for (int i = lights.size() - 1; i >= 0; i--)
	{
		const glm::vec3 lightPosition = lights[i]->GetPosition();

		const glm::vec3 directionToLightSource = (lightPosition - point).normalized();
		const float maxDistance = (point - lightPosition).length();
		std::shared_ptr<Ray> ray = std::make_shared<Ray>();
		ray->Origin = point;
		ray->Direction = directionToLightSource;

		bool shadow = sceneObject.GetScene().RayCast(sceneObjects, ray, maxDistance);

		if (shadow)
		{
			lights.erase(lights.begin() + i);
		}
	}
}
