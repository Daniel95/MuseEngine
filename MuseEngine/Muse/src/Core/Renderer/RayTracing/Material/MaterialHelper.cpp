#include "MusePCH.h"

#include "MaterialHelper.h"
#include "Core/Renderer/RayTracing/Ray.h"
#include "Core/Renderer/RayTracing/LightSource.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Scene/Scene.h"
#include "Core/Gameplay/Component/RenderComponent.h"

namespace Muse
{
	glm::vec3 operator*(const glm::vec3& a_Lhs, float a_Rhs)
	{
		return glm::vec3(a_Lhs.r * a_Rhs, a_Lhs.g * a_Rhs, a_Lhs.b * a_Rhs);
	}
}
