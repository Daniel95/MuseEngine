#include "MusePCH.h"

#include "CheckerBoardMaterial.h"
#include "SpeculairMaterial.h"
#include "DiffuseMaterial.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Scene/Scene.h"
#include "Core/Renderer/RayTracing/GetColorParameters.h"

namespace Muse
{
	CheckerBoardMaterial::CheckerBoardMaterial(const glm::vec3& a_Color1, const glm::vec3& a_Color2, float a_GridSize, float a_SpeculairStrength)
		: BlinnPhongMaterial(a_Color1, a_SpeculairStrength), m_Color2(a_Color2), m_GridSize(a_GridSize)
	{
	}

	CheckerBoardMaterial::~CheckerBoardMaterial() {}

	glm::vec3 CheckerBoardMaterial::GetColor(const GameObject& sceneObject, const glm::vec3& point, std::shared_ptr<GetColorParameters> getColorParameters) const
	{
		const glm::vec3 speculair = m_SpeculairMaterial.GetSpeculair(sceneObject, point, getColorParameters->RayDirection);
		const glm::vec3 diffuse = m_DiffuseMaterial.GetDiffuse(sceneObject, point);
		const glm::vec3 combinedLights = speculair + diffuse + sceneObject.GetScene()->GetAmbientLight();

		int gridPositionX = static_cast<int>(round(point.x / m_GridSize));
		int gridPositionY = static_cast<int>(round(point.y / m_GridSize));
		int gridPositionZ = static_cast<int>(round(point.z / m_GridSize));

		int positionInGrid = gridPositionX + gridPositionY + gridPositionZ;

		glm::vec3 result;

		if (positionInGrid % 2 == 0)
		{
			result = m_Color * combinedLights;
		}
		else
		{
			result = m_Color2 * combinedLights;
		}

		return result;
	}
}
