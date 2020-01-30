#include "MusePCH.h"

#include "MaterialHelper.h"
#include "Core/Renderer/RayTracing/Ray.h"
#include "Core/Renderer/RayTracing/LightSource.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Scene/Scene.h"

namespace Muse
{
	glm::vec3 operator*(const glm::vec3& a_Lhs, float a_Rhs)
	{
		return glm::vec3(a_Lhs.r * a_Rhs, a_Lhs.g * a_Rhs, a_Lhs.b * a_Rhs);
	}

	void FilterBlockedLights(std::vector<LightSource*>& lights, const GameObject& sceneObject, const glm::vec3& point)
	{
		std::vector<std::shared_ptr<GameObject>> sceneObjects = sceneObject.GetScene()->GetGameObjects();
		sceneObjects.erase(std::remove(sceneObjects.begin(), sceneObjects.end(), &sceneObject), sceneObjects.end());

		for (int i = lights.size() - 1; i >= 0; i--)
		{
			const glm::vec3 lightPosition = lights[i]->GetPosition();

			const glm::vec3 directionToLightSource = glm::normalize(lightPosition - point);
			const float maxDistance = (point - lightPosition).length();
			std::shared_ptr<Ray> ray = std::make_shared<Ray>();
			ray->Origin = point;
			ray->Direction = directionToLightSource;

			bool shadow = sceneObject.GetScene()->RayCast(sceneObjects, ray, maxDistance);

			if (shadow)
			{
				lights.erase(lights.begin() + i);
			}
		}
	}
}
