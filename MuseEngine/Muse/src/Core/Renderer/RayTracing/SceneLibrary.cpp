#include "MusePCH.h"

#include "Camera.h"
#include "PerspectiveCamera.h"
#include "AmbientLightSource.h"
#include "PlaneObject.h"
#include "CheckerBoardMaterial.h"
#include "ReflectiveMaterial.h"
#include "RefractiveMaterial.h"
#include "Box.h"
#include "Sphere.h"
#include "SphereObject.h"
#include "NormalDebugMaterial.h"
#include "DiffuseMaterial.h"
#include "LightSource.h"
#include "TopDownBVH.h"

#include <cassert>

Scene & GetTestingScene()
{
	const int screenSizeX = 640;
	const int screenSizeY = 480;

	PerspectiveCamera* camera = new PerspectiveCamera(glm::vec3(0, 1, -1), glm::vec3(0, -50, 100), screenSizeX, screenSizeY, 50);
	//PerspectiveCamera* camera = new PerspectiveCamera(vec3(0, 0, 0), vec3(0, 0, 2), screenSizeX, screenSizeY, vec2(15, 15), 10);

	AmbientLightSource& ambientLight = *new AmbientLightSource(glm::vec4(100, 149, 237, 255), 0.15f);
	Scene& scene = *new Scene(*camera, ambientLight, glm::vec3(0, 150, 213));

	//PLANES
	const float wallsDistance = 10;

	//PlaneObject* floor = new PlaneObject(vec3(0, -wallsDistance, 0), scene, vec3(0, 0, 0), *new BlinnPhongMaterial(glm::vec3::White));
	PlaneObject* floor = new PlaneObject(scene, glm::vec3(0, -wallsDistance, 0), glm::vec3(100, 100, 100), glm::vec3(0, 0, 0), *new CheckerBoardMaterial(glm::vec3::White, glm::vec3::Red, 2));
	//PlaneObject* ceiling = new PlaneObject(vec3(0, wallsDistance, 0), scene, vec3(0, 0, 0), *new BlinnPhongMaterial(glm::vec3::Red));
	//PlaneObject* rightWall = new PlaneObject(vec3(wallsDistance, 0, 0), scene, vec3(0, 0, 0), *new ReflectiveMaterial(glm::vec3::White));
	//PlaneObject* leftWall = new PlaneObject(vec3(-wallsDistance, 0, 0), scene, vec3(1, 0, 0), *new ReflectiveMaterial(glm::vec3::White));
	//PlaneObject* frontWall = new PlaneObject(vec3(0, 0, 15), scene, vec3(0, 0, 0), *new BlinnPhongMaterial(glm::vec3::White));

	scene.AddSceneObject(*floor);
	//scene.AddSceneObject(*ceiling);
	//scene.AddSceneObject(*rightWall);
	//scene.AddSceneObject(*leftWall);
	//scene.AddSceneObject(*frontWall);

	//SPHERES:
	SceneObject* box = new SceneObject(scene, *new ReflectiveMaterial(glm::vec3(100, 149, 237)), *new Box(glm::vec3(2, -1, 7)));
	SphereObject* sphereObject1 = new SphereObject(scene, glm::vec3(-2, -1.5, 7), 2, *new ReflectiveMaterial(glm::vec3(100, 149, 237)));
	SphereObject* sphereObject2 = new SphereObject(scene, glm::vec3(1, -3, 4), 1, *new BlinnPhongMaterial(glm::vec3(106, 90, 205)));

	scene.AddSceneObject(*box);
	scene.AddSceneObject(*sphereObject1);
	scene.AddSceneObject(*sphereObject2);

	LightSource& pointLight1 = *new LightSource(glm::vec3(-3, 0, 0), glm::vec4(1.f, 1.f, 1.f, 1.f), 0.7f, 60);
	//PointLightSource& pointLight1 = *new PointLightSource(vec3(0, 0, 0), glm::vec3::Yellow, 0.7f, 60);
	LightSource& pointLight2 = *new LightSource(glm::vec3(3, 0, 0), glm::vec4(0.f, 1.f, 1.f, 1.f), 0.7f, 60);

	scene.AddLight(pointLight1);
	scene.AddLight(pointLight2);

	return scene;
}

Scene & GetWhittedImageScene()
{
	const int screenSizeX = 640;
	const int screenSizeY = 480;

	PerspectiveCamera* camera = new PerspectiveCamera(vec3(0, 0, -1), vec3(0, 0, 100), screenSizeX, screenSizeY, 50);

	glm::vec3 ambientLightColor = glm::vec3(255, 255, 255);
	float ambientLightIntensity = 0.9f;
	AmbientLightSource& ambientLight = *new AmbientLightSource(ambientLightColor, ambientLightIntensity);
	Scene& scene = *new Scene(*camera, ambientLight, glm::vec3(0, 150, 213) * ambientLightColor * ambientLightIntensity);

	//PLANES
	const float wallsDistance = 10;

	PlaneObject* floor = new PlaneObject(scene, vec3(0, -wallsDistance, 0), vec3(100, 100, 100), vec3(0, 0, 0), *new CheckerBoardMaterial(glm::vec3::Yellow, glm::vec3::Red, 5));

	scene.AddSceneObject(*floor);

	//SPHERES:
	SphereObject* sphereObject1 = new SphereObject(scene, vec3(0, 0.5f, 3), 1, *new RefractiveMaterial(glm::vec3::White, 1, 0.9f));
	SphereObject* sphereObject2 = new SphereObject(scene, vec3(1.7f, -0.3f, 6), 1, *new ReflectiveMaterial(glm::vec3::White, 0.5f, 0.3f));

	scene.AddSceneObject(*sphereObject1);
	scene.AddSceneObject(*sphereObject2);

	LightSource& pointLight1 = *new LightSource(vec3(0, 3, 0), glm::vec3::White, 1, 100);

	scene.AddLight(pointLight1);

	return scene;
}

Scene & GetOBBTestScene()
{
	const int screenSizeX = 640;
	const int screenSizeY = 480;

	PerspectiveCamera* camera = new PerspectiveCamera(vec3(0, 0, -2), vec3(0, 0, 100), screenSizeX, screenSizeY, 50);

	glm::vec3 ambientLightColor = glm::vec3(255, 255, 255);
	float ambientLightIntensity = 0.5f;
	AmbientLightSource& ambientLight = *new AmbientLightSource(ambientLightColor, ambientLightIntensity);
	Scene& scene = *new Scene(*camera, ambientLight, glm::vec3(0, 150, 213) * ambientLightColor * ambientLightIntensity);

	//PLANES
	//PlaneObject* floor = new PlaneObject(scene, vec3(0, -10, 0), vec3(100, 100, 100), vec3(0, 0, 0), *new CheckerBoardMaterial());
	//scene.AddSceneObject(*floor);

	//Objects:
	SceneObject* box1 = new SceneObject(scene, *new NormalDebugMaterial(), *new Box(vec3(-1, 0.0f, 0.0f)));
	SceneObject* box2 = new SceneObject(scene, *new NormalDebugMaterial(), *new Box(vec3(1, 0.0f, 0.0f)));
	SceneObject* box3 = new SceneObject(scene, *new NormalDebugMaterial(), *new Box(vec3(0, 0.0f, -10)));

	box2->GetShape().GetTransform().Rotate(vec3(0, 45, 45));

	scene.AddSceneObject(*box1);
	scene.AddSceneObject(*box2);
	scene.AddSceneObject(*box3);

	LightSource& pointLight1 = *new LightSource(vec3(0, 0, -2), glm::vec3::White, 1, 100);

	scene.AddLight(pointLight1);

	return scene;
}

Scene & ReferenceImage2()
{
	const int screenSizeX = 640;
	const int screenSizeY = 480;

	PerspectiveCamera* camera = new PerspectiveCamera(vec3(0, 10, -15), vec3(0, 0, 0), screenSizeX, screenSizeY, 50);

	glm::vec3 ambientLightColor = glm::vec3(255, 255, 255);
	float ambientLightIntensity = 0.3f;
	AmbientLightSource& ambientLight = *new AmbientLightSource(ambientLightColor, ambientLightIntensity);
	Scene& scene = *new Scene(*camera, ambientLight, glm::vec3(0, 150, 213) * ambientLightColor * ambientLightIntensity);
	std::shared_ptr<TopDownBVH> topDownBVH = std::make_shared<TopDownBVH>(scene);
	scene.SetBVH(topDownBVH);

	float maxSpawnCircleSize = 40;
	float minSpawnCircleSize = 8;
	float spawnCircleSizeRange = maxSpawnCircleSize - minSpawnCircleSize;
	float minSize = 1;
	float maxSize = 4;
	float sizeRange = maxSize - minSize;

	for (int i = 0; i < 100; i++)
	{
		float randomX = static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / 2.0f) - 1.0f;
		float randomZ = static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / 2.0f) - 1.0f;
		vec3 randomDirection = vec3(randomX, 0, randomZ).normalized();
		float randomDistance = static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / spawnCircleSizeRange) + minSpawnCircleSize;
		vec3 position = randomDirection * randomDistance;

		glm::vec3 randomColor = glm::vec3(
			static_cast<sf::Uint8>(255 * static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX + 1u)),
			static_cast<sf::Uint8>(255 * static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX + 1u)),
			static_cast<sf::Uint8>(255 * static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX + 1u)));

		SceneObject* box = new SceneObject(scene, *new BlinnPhongMaterial(randomColor, 0.7f), *new Box(position));

		box->GetShape().GetTransform().Rotate(vec3(
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / 360),
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / 360),
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / 360)
		));
		box->GetShape().GetTransform().Scale(vec3(
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / sizeRange) + minSize,
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / sizeRange) + minSize,
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / sizeRange) + minSize
		));

		scene.AddSceneObject(*box);
	}

	//SPHERE
	SceneObject* sphere = new SceneObject(scene, *new ReflectiveMaterial(glm::vec3::Yellow, 0.2f), *new Sphere(vec3(0, 0.0f, 0.0f), 2));
	scene.AddSceneObject(*sphere);

	//FLOOR
	SceneObject* floor = new SceneObject(scene, *new BlinnPhongMaterial(glm::vec3::White), *new Plane(vec3(0, -10, 0), vec3(0, 1, 0), vec3(100, 100, 100)));
	scene.AddSceneObject(*floor);

	LightSource& pointLight1 = *new LightSource(vec3(0, 5, 0), glm::vec3::White, 1, 100);
	scene.AddLight(pointLight1);

	scene.ConstructBVH();
	return scene;
}

Scene & GetPerformanceTestScene1(bool bvh, int boxesAmount)
{
	const int screenSizeX = 640;
	const int screenSizeY = 480;

	PerspectiveCamera* camera = new PerspectiveCamera(vec3(0, 10, -15), vec3(0, 0, 0), screenSizeX, screenSizeY, 50);

	glm::vec3 ambientLightColor = glm::vec3(255, 255, 255);
	float ambientLightIntensity = 0.3f;
	AmbientLightSource& ambientLight = *new AmbientLightSource(ambientLightColor, ambientLightIntensity);
	Scene& scene = *new Scene(*camera, ambientLight, glm::vec3(0, 150, 213) * ambientLightColor * ambientLightIntensity);
	if (bvh)
	{
		std::shared_ptr<TopDownBVH> topDownBVH = std::make_shared<TopDownBVH>(scene);
		scene.SetBVH(topDownBVH);
	}

	float maxSpawnCircleSize = 40;
	float minSpawnCircleSize = 8;
	float spawnCircleSizeRange = maxSpawnCircleSize - minSpawnCircleSize;
	float minSize = 1;
	float maxSize = 4;
	float sizeRange = maxSize - minSize;

	for (int i = 0; i < boxesAmount; i++)
	{
		float randomX = static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / 2.0f) - 1.0f;
		float randomZ = static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / 2.0f) - 1.0f;
		vec3 randomDirection = vec3(randomX, 0, randomZ).normalized();
		float randomDistance = static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / spawnCircleSizeRange) + minSpawnCircleSize;
		vec3 position = randomDirection * randomDistance;

		glm::vec3 randomColor = glm::vec3(
			static_cast<sf::Uint8>(255 * static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX + 1u)), 
			static_cast<sf::Uint8>(255 * static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX + 1u)), 
			static_cast<sf::Uint8>(255 * static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX + 1u)));

		SceneObject* box = new SceneObject(scene, *new BlinnPhongMaterial(randomColor, 0.7f), *new Box(position));

		box->GetShape().GetTransform().Rotate(vec3(
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / 360),
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / 360),
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / 360)
		));
		box->GetShape().GetTransform().Scale(vec3(
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / sizeRange) + minSize,
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / sizeRange) + minSize,
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / sizeRange) + minSize
		));

		scene.AddSceneObject(*box);
	}

	if (bvh)
	{
		scene.ConstructBVH();
	}
	return scene;
}

Scene & GetPerformanceTestScene2(bool bvh, int boxesAmount)
{
	const int screenSizeX = 640;
	const int screenSizeY = 480;

	PerspectiveCamera* camera = new PerspectiveCamera(vec3(0, 10, -15), vec3(0, 0, 0), screenSizeX, screenSizeY, 50);

	glm::vec3 ambientLightColor = glm::vec3(255, 255, 255);
	float ambientLightIntensity = 0.3f;
	AmbientLightSource& ambientLight = *new AmbientLightSource(ambientLightColor, ambientLightIntensity);
	Scene& scene = *new Scene(*camera, ambientLight, glm::vec3(0, 150, 213) * ambientLightColor * ambientLightIntensity);
	if (bvh)
	{
		std::shared_ptr<TopDownBVH> topDownBVH = std::make_shared<TopDownBVH>(scene);
		scene.SetBVH(topDownBVH);
	}

	float spawnBoxSize = static_cast<float>(boxesAmount);
	float minSize = 1;
	float maxSize = 4;
	float sizeRange = maxSize - minSize;

	for (int i = 0; i < boxesAmount; i++)
	{
		float randomX = static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / 2.0f) - 1.0f;
		float randomY = static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / 2.0f) - 1.0f;
		float randomZ = static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / 2.0f) - 1.0f;
		vec3 position = vec3(randomX, randomY, randomZ) * spawnBoxSize;

		glm::vec3 randomColor = glm::vec3(
			static_cast<sf::Uint8>(255 * static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX + 1u)),
			static_cast<sf::Uint8>(255 * static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX + 1u)),
			static_cast<sf::Uint8>(255 * static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX + 1u)));

		SceneObject* box = new SceneObject(scene, *new BlinnPhongMaterial(randomColor, 0.7f), *new Box(position));

		box->GetShape().GetTransform().Rotate(vec3(
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / 360),
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / 360),
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / 360)
		));
		box->GetShape().GetTransform().Scale(vec3(
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / sizeRange) + minSize,
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / sizeRange) + minSize,
			static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX + 1u) / sizeRange) + minSize
		));

		scene.AddSceneObject(*box);
	}

	LightSource& pointLight1 = *new LightSource(vec3(0, 5, 0), glm::vec3::White, 1, 100);
	scene.AddLight(pointLight1);

	if (bvh)
	{
		scene.ConstructBVH();
	}
	return scene;
}