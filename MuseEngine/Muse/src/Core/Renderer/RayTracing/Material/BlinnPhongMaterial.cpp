#include "MusePCH.h"

#include "BlinnPhongMaterial.h"
#include "SpecularMaterial.h"
#include "DiffuseMaterial.h"
#include "Core/Scene/Scene.h"
#include "Core/Renderer/RayTracing/GetColorParameters.h"

namespace Muse
{
	BlinnPhongMaterial::BlinnPhongMaterial(const glm::vec3& a_Color, float a_SpeculairStrength)
		: m_Color(a_Color), m_SpeculairMaterial(*new SpecularMaterial(a_Color, a_SpeculairStrength)), m_DiffuseMaterial(*new DiffuseMaterial(a_Color))
	{
	}

	glm::vec3 BlinnPhongMaterial::GetColor(std::shared_ptr<const RenderComponent> a_RenderComponent, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const
	{
		const glm::vec3 speculair = m_SpeculairMaterial.GetSpecular(a_RenderComponent, point, getColorParameters->RayDirection);
		const glm::vec3 diffuse = m_DiffuseMaterial.GetDiffuse(a_RenderComponent, point);
		const glm::vec3 combinedLights = speculair + diffuse + 0.1f;//a_gameObject.GetScene().GetAmbientLight();
		const glm::vec3 result = m_Color * combinedLights;

		return result;
	}
}