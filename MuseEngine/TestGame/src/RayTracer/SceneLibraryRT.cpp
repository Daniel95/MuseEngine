#include "SceneLibraryRT.h"
#include "Core/Renderer/RayTracing/AmbientLightSource.h"
#include "Core/Scene/Scene.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/RenderComponent.h"
#include "Core/Renderer/RayTracing/Shape/Sphere.h"
#include "Core/Renderer/RayTracing/Material/DiffuseMaterial.h"
#include "Core/Renderer/RayTracing/Material/ReflectiveMaterial.h"
#include "Core/Renderer/RayTracing/Shape/Plane.h"
#include "Core/Renderer/RayTracing/Material/CheckerBoardMaterial.h"
#include "Core/Renderer/RayTracing/Material/SpecularMaterial.h"
#include "Core/Renderer/RayTracing/Material/RefractiveMaterial.h"

void SceneLibraryRT::MakeRTBenchmarkScene(std::shared_ptr<Muse::Scene> a_Scene)
{
	std::shared_ptr<Muse::AmbientLightSource> ambientLight = std::make_shared<Muse::AmbientLightSource>(glm::vec3(1.0f), 0.25f);

	a_Scene->SetBackgroundColor(glm::vec3(0.3f));

	{
		std::shared_ptr<Muse::LightSource> lightSource = std::make_shared<Muse::LightSource>(glm::vec3(-2, 3, 0), glm::vec3(0.1f, 0.f, 1), 1.5f, 10);
		a_Scene->AddLightSource(lightSource);
	}

	{
		//std::shared_ptr<Muse::LightSource> lightSource = std::make_shared<Muse::LightSource>(glm::vec3(2, 3, 0), glm::vec3(0.3f, 0.6f, 0.9f), 1.5f, 10);
		//a_Scene->AddLightSource(lightSource);
	}

	auto diffuseMaterialComponent = std::make_shared<Muse::DiffuseMaterial>(glm::vec3(0.8f, 0.3f, 0.5f));
	auto blinnPhongMaterialComponent = std::make_shared<Muse::BlinnPhongMaterial>(glm::vec3(0.8f, 0.3f, 0.5f));
	auto reflectiveMaterial = std::make_shared<Muse::ReflectiveMaterial>();
	//auto groundReflectiveMaterial = std::make_shared<Muse::ReflectiveMaterial>(glm::vec3(0), 0.5f,0.1f);
	auto speculairMaterial = std::make_shared<Muse::SpecularMaterial>();
	auto refractiveMaterial = std::make_shared<Muse::RefractiveMaterial>();
	auto checkerBoardMaterial = std::make_shared<Muse::CheckerBoardMaterial>();

	a_Scene->SetAmbientLight(ambientLight);

	{
		auto sphereGameObject = a_Scene->AddGameObject(glm::vec3(0, 0, 5), glm::vec3(1, 1, 1));
		auto sphereComponent = sphereGameObject->AddComponent<Muse::Sphere>();
		sphereComponent->SetRadius(1.f);

		auto renderComponent = sphereGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(reflectiveMaterial);
		renderComponent->SetShape(sphereComponent);
	}

	{
		auto sphereGameObject = a_Scene->AddGameObject(glm::vec3(-3, 0, 5), glm::vec3(1, 1, 1));
		auto sphereComponent = sphereGameObject->AddComponent<Muse::Sphere>();
		sphereComponent->SetRadius(1.f);

		auto renderComponent = sphereGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(refractiveMaterial);
		renderComponent->SetShape(sphereComponent);
	}

	{
		auto sphereGameObject = a_Scene->AddGameObject(glm::vec3(3, 0, 5), glm::vec3(1, 1, 1));
		auto sphereComponent = sphereGameObject->AddComponent<Muse::Sphere>();
		sphereComponent->SetRadius(1.f);

		auto renderComponent = sphereGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(blinnPhongMaterialComponent);
		renderComponent->SetShape(sphereComponent);
	}

	{
		auto planeGameObject = a_Scene->AddGameObject(glm::vec3(0, -3, 5), glm::vec3(15));
		auto planeComponent = planeGameObject->AddComponent<Muse::Plane>();
		planeComponent->SetNormal(glm::vec3(0, 1, 0));

		auto renderComponent = planeGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(checkerBoardMaterial);
		renderComponent->SetShape(planeComponent);
	}
}

void SceneLibraryRT::MakePTBenchmarkScene(std::shared_ptr<Muse::Scene> a_Scene)
{
	std::shared_ptr<Muse::AmbientLightSource> ambientLight = std::make_shared<Muse::AmbientLightSource>(glm::vec3(1.0f), 0.25f);

	a_Scene->SetBackgroundColor(glm::vec3(0.3f));

	{
		std::shared_ptr<Muse::LightSource> lightSource = std::make_shared<Muse::LightSource>(glm::vec3(-2, 3, 0), glm::vec3(0.1f, 0.f, 1), 1.5f, 10);
		a_Scene->AddLightSource(lightSource);
	}

	{
		//std::shared_ptr<Muse::LightSource> lightSource = std::make_shared<Muse::LightSource>(glm::vec3(2, 3, 0), glm::vec3(0.3f, 0.6f, 0.9f), 1.5f, 10);
		//a_Scene->AddLightSource(lightSource);
	}

	auto redDiffuseMaterialComponent = std::make_shared<Muse::DiffuseMaterial>(glm::vec3(1.f, 0.f, 0.f));
	auto greenDiffuseMaterialComponent = std::make_shared<Muse::DiffuseMaterial>(glm::vec3(0.f, 1.f, 0.f));
	auto blueDiffuseMaterialComponent = std::make_shared<Muse::DiffuseMaterial>(glm::vec3(0.f, 0.f, 1.0f));
	auto blinnPhongMaterialComponent = std::make_shared<Muse::BlinnPhongMaterial>(glm::vec3(0.8f, 0.3f, 0.5f));
	auto reflectiveMaterial = std::make_shared<Muse::ReflectiveMaterial>();
	//auto groundReflectiveMaterial = std::make_shared<Muse::ReflectiveMaterial>(glm::vec3(0), 0.5f,0.1f);
	auto speculairMaterial = std::make_shared<Muse::SpecularMaterial>();
	auto refractiveMaterial = std::make_shared<Muse::RefractiveMaterial>();
	auto checkerBoardMaterial = std::make_shared<Muse::CheckerBoardMaterial>();

	a_Scene->SetAmbientLight(ambientLight);

	{
		auto sphereGameObject = a_Scene->AddGameObject(glm::vec3(0, 0, 5), glm::vec3(1, 1, 1));
		auto sphereComponent = sphereGameObject->AddComponent<Muse::Sphere>();
		sphereComponent->SetRadius(1.f);

		auto renderComponent = sphereGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(redDiffuseMaterialComponent);
		renderComponent->SetShape(sphereComponent);
	}

	{
		auto sphereGameObject = a_Scene->AddGameObject(glm::vec3(-3, 0, 5), glm::vec3(1, 1, 1));
		auto sphereComponent = sphereGameObject->AddComponent<Muse::Sphere>();
		sphereComponent->SetRadius(1.f);

		auto renderComponent = sphereGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(greenDiffuseMaterialComponent);
		renderComponent->SetShape(sphereComponent);
	}

	{
		auto sphereGameObject = a_Scene->AddGameObject(glm::vec3(3, 0, 5), glm::vec3(1, 1, 1));
		auto sphereComponent = sphereGameObject->AddComponent<Muse::Sphere>();
		sphereComponent->SetRadius(1.f);

		auto renderComponent = sphereGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(blueDiffuseMaterialComponent);
		renderComponent->SetShape(sphereComponent);
	}

	MakePtSkyBox(a_Scene, 15, 10, 15);
}

void SceneLibraryRT::MakePtSkyBox(std::shared_ptr<Muse::Scene> a_Scene, float a_Width, float a_Height, float a_Length)
{
	float x = a_Width / 2;
	//float y = a_Height / 2;
	float z = a_Length / 2;

	auto greenDiffuseMaterialComponent = std::make_shared<Muse::DiffuseMaterial>(glm::vec3(0.f, 1.f, 0.f));
	auto checkerBoardMaterial = std::make_shared<Muse::CheckerBoardMaterial>();

	{
		auto planeGameObject = a_Scene->AddGameObject(glm::vec3(x, 5, 0), glm::vec3(15));
		auto planeComponent = planeGameObject->AddComponent<Muse::Plane>();
		planeComponent->SetNormal(glm::vec3(-1, 0, 0));

		auto renderComponent = planeGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(greenDiffuseMaterialComponent);
		renderComponent->SetShape(planeComponent);
	}

	{
		auto planeGameObject = a_Scene->AddGameObject(glm::vec3(-x, 0, 0), glm::vec3(15));
		auto planeComponent = planeGameObject->AddComponent<Muse::Plane>();
		planeComponent->SetNormal(glm::vec3(1, 0, 0));

		auto renderComponent = planeGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(greenDiffuseMaterialComponent);
		renderComponent->SetShape(planeComponent);
	}

	{
		auto planeGameObject = a_Scene->AddGameObject(glm::vec3(0, a_Height, 0), glm::vec3(15));
		auto planeComponent = planeGameObject->AddComponent<Muse::Plane>();
		planeComponent->SetNormal(glm::vec3(0, -1, 0));

		auto renderComponent = planeGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(checkerBoardMaterial);
		renderComponent->SetShape(planeComponent);
	}

	{
		auto planeGameObject = a_Scene->AddGameObject(glm::vec3(0, 0, 0), glm::vec3(15));
		auto planeComponent = planeGameObject->AddComponent<Muse::Plane>();
		planeComponent->SetNormal(glm::vec3(0, 1, 0));

		auto renderComponent = planeGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(checkerBoardMaterial);
		renderComponent->SetShape(planeComponent);
	}

	/*
	{
		auto planeGameObject = a_Scene->AddGameObject(glm::vec3(0, 0, -z), glm::vec3(15));
		auto planeComponent = planeGameObject->AddComponent<Muse::Plane>();
		planeComponent->SetNormal(glm::vec3(0, 0, 1));

		auto renderComponent = planeGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(greenDiffuseMaterialComponent);
		renderComponent->SetShape(planeComponent);
	}

	{
		auto planeGameObject = a_Scene->AddGameObject(glm::vec3(0, 0, z), glm::vec3(15));
		auto planeComponent = planeGameObject->AddComponent<Muse::Plane>();
		planeComponent->SetNormal(glm::vec3(0, 0, -1));

		auto renderComponent = planeGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(greenDiffuseMaterialComponent);
		renderComponent->SetShape(planeComponent);
	}
    */
}
