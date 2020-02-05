#include "MusePCH.h"

#include "BlinnPhongMaterial.h"
#include "SpecularMaterial.h"
#include "DiffuseMaterial.h"
#include "Core/Scene/Scene.h"
#include "Core/Renderer/RayTracing/GetColorParameters.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Renderer/RayTracing/LightSource.h"
#include "Core/Renderer/RayTracing/Ray.h"
#include "Core/Gameplay/Component/RenderComponent.h"

namespace Muse
{
	BlinnPhongMaterial::BlinnPhongMaterial(const glm::vec3& a_Color, float a_SpeculairStrength)
		: m_Color(a_Color), m_SpeculairMaterial(*new SpecularMaterial(a_Color, a_SpeculairStrength)), m_DiffuseMaterial(*new DiffuseMaterial(a_Color))
	{
	}

	glm::vec3 BlinnPhongMaterial::GetColor(const std::shared_ptr<const RenderComponent>& a_RenderComponent, const glm::vec3& a_Point, const std::shared_ptr<GetColorParameters>& a_GetColorParameters) const
	{
		const glm::vec3 blinnPhong = GetBlinnPhong(a_RenderComponent, a_Point, a_GetColorParameters);
		const glm::vec3 result = m_Color * blinnPhong;

		return result;
	}

    glm::vec3 BlinnPhongMaterial::GetBlinnPhong(std::shared_ptr<const RenderComponent> a_RenderComponent,
        const glm::vec3& a_Point, const std::shared_ptr<GetColorParameters>& a_GetColorParameters) const
    {
		glm::vec3 specularAndDiffuse = glm::vec3(0);
		glm::vec3 normal = a_RenderComponent->GetNormal(a_Point);

		const std::vector<std::shared_ptr<LightSource>>& lightSources = SceneManager::GetActiveScene()->GetLightSources();

		Ray ray{ a_Point };

		for (const std::shared_ptr<LightSource>& lightSource : lightSources)
		{
			const glm::vec3 lightPosition = lightSource->GetPosition();
			const glm::vec3 directionToLightSource = glm::normalize(lightPosition - a_Point);
			glm::vec3 light = lightSource->GetLight(a_Point);

			ray.Direction = directionToLightSource;



			if (!ray.Cast())
			{
				float specular = m_SpeculairMaterial.GetSpecular(normal, a_GetColorParameters->RayDirection, directionToLightSource);
				float diffuseT = DiffuseMaterial::GetDiffuse(normal, directionToLightSource);

				specularAndDiffuse += (specular + diffuseT) * light;
			}
		}

		const glm::vec3 combinedLights = specularAndDiffuse + SceneManager::GetActiveScene()->GetAmbientLight();

		return combinedLights;
    }
}
