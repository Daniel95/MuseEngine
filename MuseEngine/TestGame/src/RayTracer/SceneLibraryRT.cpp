#include "SceneLibraryRT.h"
#include "Core/Renderer/RayTracing/AmbientLightSource.h"
#include "Core/Scene/Scene.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/RenderComponent.h"
#include "Core/Renderer/RayTracing/Shape/Sphere.h"
#include "Core/Renderer/RayTracing/Material/DiffuseMaterial.h"

void SceneLibraryRT::MakeTestScene(std::shared_ptr<Muse::Scene> a_Scene)
{
	std::shared_ptr<Muse::AmbientLightSource> ambientLight = std::make_shared<Muse::AmbientLightSource>(glm::vec3(1, 1, 1), 0.15f);

	a_Scene->SetBackgroundColor(glm::vec3(1));

	std::shared_ptr<Muse::LightSource> lightSource = std::make_shared<Muse::LightSource>(glm::vec3(0, 3, 1), glm::vec3(1, 1,1), 1, 10);
	a_Scene->AddLightSource(lightSource);

	auto diffuseMaterialComponent= std::make_shared<Muse::DiffuseMaterial>(glm::vec3(0.8f, 0.3f, 0.5f));

	a_Scene->SetAmbientLight(ambientLight);

	{
		auto sphereGameObject = a_Scene->AddGameObject(glm::vec3(0, -3, 5), glm::vec3(1, 1, 1));
		auto sphereComponent = sphereGameObject->AddComponent<Muse::Sphere>();
		sphereComponent->SetRadius(3.f);

		auto renderComponent = sphereGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(diffuseMaterialComponent);
		renderComponent->SetShape(sphereComponent);
	}
}
