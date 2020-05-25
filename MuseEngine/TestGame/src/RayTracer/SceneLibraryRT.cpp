#include "SceneLibraryRT.h"
#include "Core/Renderer/RayTracer/AmbientLightSource.h"
#include "Core/Scene/Scene.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/RenderComponent.h"
#include "Core/Renderer/RayTracer/Shape/Sphere.h"
#include "Core/Renderer/RayTracer/Shape/Plane.h"
#include "Core/Renderer/RayTracer/Shape/Box.h"

void SceneLibraryRT::MakeRTBenchmarkScene(std::shared_ptr<Muse::Scene> a_Scene)
{
	std::shared_ptr<Muse::AmbientLightSource> ambientLight = std::make_shared<Muse::AmbientLightSource>(glm::vec3(1.0f), 0.25f);

	a_Scene->SetBackgroundColor(glm::vec3(0.1f, 0.1f, 0.25f));

	{
		std::shared_ptr<Muse::LightSource> lightSource = std::make_shared<Muse::LightSource>(glm::vec3(-2, 3, 0), glm::vec3(0.1f, 0.f, 1), 1.5f, 10);
		a_Scene->AddLightSource(lightSource);
	}

	{
		//std::shared_ptr<Muse::LightSource> lightSource = std::make_shared<Muse::LightSource>(glm::vec3(2, 3, 0), glm::vec3(0.3f, 0.6f, 0.9f), 1.5f, 10);
		//a_Scene->AddLightSource(lightSource);
	}


	Muse::Material diffuseMaterial = { Muse::MaterialType::Diffuse, glm::vec3(0.8f, 0.3f, 0.5f) };
	Muse::Material speculairMaterial = { Muse::MaterialType::Specular};
	Muse::Material blinnPhongMaterialComponent = { Muse::MaterialType::BlinnPhong, glm::vec3(0.8f, 0.3f, 0.5f) };
	Muse::Material reflectiveMaterial = { Muse::MaterialType::Reflective };
	Muse::Material refractiveMaterial = { Muse::MaterialType::Refractive };
	Muse::Material checkerBoardMaterial = { Muse::MaterialType::Checkerboard };
	Muse::Material normalDebugMaterial = { Muse::MaterialType::NormalDebug };

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

	{
		auto planeGameObject = a_Scene->AddGameObject(glm::vec3(-2, 1.5f, 6), glm::vec3(0.5f, 3, 2));
		planeGameObject->GetTransform()->RotateLocal(glm::vec3(111, 512, 13));
		auto boxComponent = planeGameObject->AddComponent<Muse::Box>();

		auto renderComponent = planeGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(normalDebugMaterial);
		renderComponent->SetShape(boxComponent);
	}
}

void SceneLibraryRT::MakePTBenchmarkScene(std::shared_ptr<Muse::Scene> a_Scene)
{
	std::shared_ptr<Muse::AmbientLightSource> ambientLight = std::make_shared<Muse::AmbientLightSource>(glm::vec3(1.0f), 0.25f);

	a_Scene->SetBackgroundColor(glm::vec3(0.3f));

	Muse::Material redDiffuseMaterial = { Muse::MaterialType::Diffuse, glm::vec3(0.8f, 0.3f, 0.5f) };
	Muse::Material greenDiffuseMaterial = { Muse::MaterialType::Diffuse, glm::vec3(0.f, 1.f, 0.f) };
	Muse::Material blueDiffuseMaterial = { Muse::MaterialType::Diffuse, glm::vec3(0.f, 0.f, 1.0f) };

	a_Scene->SetAmbientLight(ambientLight);

	{
		auto sphereGameObject = a_Scene->AddGameObject(glm::vec3(-2.5f, -1.2, 5), glm::vec3(1, 1, 1));
		auto sphereComponent = sphereGameObject->AddComponent<Muse::Sphere>();
		sphereComponent->SetRadius(1.f);

		auto renderComponent = sphereGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(redDiffuseMaterial);
		renderComponent->SetShape(sphereComponent);
	}

	{
		auto sphereGameObject = a_Scene->AddGameObject(glm::vec3(0, -1.7, 5), glm::vec3(1, 1, 1));
		auto sphereComponent = sphereGameObject->AddComponent<Muse::Sphere>();
		sphereComponent->SetRadius(1.f);

		auto renderComponent = sphereGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(greenDiffuseMaterial);
		renderComponent->SetShape(sphereComponent);
	}

	{
		auto sphereGameObject = a_Scene->AddGameObject(glm::vec3(2.5f, -2.2, 5), glm::vec3(1, 1, 1));
		auto sphereComponent = sphereGameObject->AddComponent<Muse::Sphere>();
		sphereComponent->SetRadius(1.f);

		auto renderComponent = sphereGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(blueDiffuseMaterial);
		renderComponent->SetShape(sphereComponent);
	}

	/*
	{
		auto planeGameObject = a_Scene->AddGameObject(glm::vec3(2, 0, 0), glm::vec3(0.5f, 1.f, 1.5f));
		planeGameObject->GetTransform()->RotateLocal(glm::vec3(23, 5, 88));
		auto boxComponent = planeGameObject->AddComponent<Muse::Box>();

		auto renderComponent = planeGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(blinnPhongMaterialComponent);
		renderComponent->SetShape(boxComponent);
	}
    */

	MakePtSkyBox(a_Scene, 15, 3, 15);
}

void SceneLibraryRT::MakePtSkyBox(std::shared_ptr<Muse::Scene> a_Scene, float a_Width, float a_Height, float a_Length)
{
	//float x = a_Width / 2;
	float y = a_Height / 2;
	//float z = a_Length / 2;

	Muse::Material checkerBoardMaterial = { Muse::MaterialType::Checkerboard };
	Muse::Material lightMaterial = { Muse::MaterialType::Light, glm::vec3(1.f, 1.f, 0.6f) };

	{
		auto boxGameObject = a_Scene->AddGameObject(glm::vec3(2, y, 8), glm::vec3(2.f, 2, 1.5));
		auto boxComponent = boxGameObject->AddComponent<Muse::Box>();

		auto renderComponent = boxGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(lightMaterial);
		renderComponent->SetShape(boxComponent);
	}

	{
		auto boxGameObject = a_Scene->AddGameObject(glm::vec3(-1.5f, y, 6), glm::vec3(2, 1, 1.5f));
		auto boxComponent = boxGameObject->AddComponent<Muse::Box>();

		auto renderComponent = boxGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(lightMaterial);
		renderComponent->SetShape(boxComponent);
	}

	/*
	{
		auto boxGameObject = a_Scene->AddGameObject(glm::vec3(-4, 0, -3), glm::vec3(3, 5, 1));
		auto boxComponent = boxGameObject->AddComponent<Muse::Box>();

		auto renderComponent = boxGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(whiteDiffuseMaterialComponent);
		renderComponent->SetShape(boxComponent);
	}
    */

	{
		auto planeGameObject = a_Scene->AddGameObject(glm::vec3(0, -3, 0), glm::vec3(15));
		auto planeComponent = planeGameObject->AddComponent<Muse::Plane>();
		planeComponent->SetNormal(glm::vec3(0, 1, 0));

		auto renderComponent = planeGameObject->AddComponent<Muse::RenderComponent>();

		renderComponent->SetMaterial(checkerBoardMaterial);
		renderComponent->SetShape(planeComponent);
	}
}
