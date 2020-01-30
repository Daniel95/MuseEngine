#include "SceneLibraryRT.h"
#include "Core/Renderer/RayTracing/AmbientLightSource.h"
#include "Core/Scene/Scene.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/RenderComponent.h"
#include "Core/Renderer/RayTracing/Shape/Sphere.h"
#include "Core/Renderer/RayTracing/Material/DiffuseMaterial.h"

void SceneLibraryRT::MakeTestScene(std::shared_ptr<Muse::Scene> a_Scene)
{
	std::shared_ptr<Muse::AmbientLightSource> ambientLight = std::make_shared<Muse::AmbientLightSource>(glm::vec3(100, 149, 237), 0.15f);

	auto diffuseMaterialComponent= std::make_shared<Muse::DiffuseMaterial>(glm::vec3(0.8f, 0.3f, 0.5f));

	a_Scene->SetAmbientLight(ambientLight);

	{
		auto sphereGameObject = a_Scene->AddGameObject(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1));
		auto sphereComponent = sphereGameObject->AddComponent<Muse::Sphere>();
		sphereComponent->SetRadius(99999999999999999);

		auto renderComponent = sphereGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(diffuseMaterialComponent);
		renderComponent->SetShape(sphereComponent);
	}
}
