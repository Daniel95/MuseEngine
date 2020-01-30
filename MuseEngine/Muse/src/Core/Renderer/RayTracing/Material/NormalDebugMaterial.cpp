#include "MusePCH.h"

#include "NormalDebugMaterial.h"
#include "MaterialHelper.h"

namespace Muse
{
	NormalDebugMaterial::NormalDebugMaterial(const glm::vec3& a_Color)
		: m_Color(a_Color)
	{
	}

	glm::vec3 NormalDebugMaterial::GetColor(const GameObject& a_GameObject, const glm::vec3& a_Point, std::shared_ptr<GetColorParameters> a_GetColorParameters) const
	{
		a_GetColorParameters;


		//glm::vec3 normal = a_GameObject.GetNormal(point);
		glm::vec3 objNormal = glm::vec3();//a_GameObject.GetNormal(point);

		const glm::vec3 normal = (objNormal + glm::vec3(1, 1, 1)) / 2.f;
		const glm::vec3 normalDebugColor = glm::vec3(normal.x * m_Color.r, normal.y * m_Color.g, normal.z * m_Color.b);
		return normalDebugColor;
	}
}