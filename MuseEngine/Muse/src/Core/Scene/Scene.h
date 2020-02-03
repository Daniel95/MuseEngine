#pragma once

#include "Core/Utilities/Defines.h"
#include "Core/Resource/Resource.h"
#include "Core/Instrumentor.h"

#include <rttr/variant.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <deque>
#include <string>
#include <vector>
#include <memory>

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

namespace Muse 
{
    class RenderComponent;
    struct Ray;
    struct RayHitData;
    class Component;
    class GameObject;
    class TransformComponent;
    class CameraComponent;
    class Application;
    class AmbientLightSource;
    class BVH;
    class LightSource;

	class Scene : public Resource
	{
	public:
		Scene();
        virtual ~Scene();

        void DestroyAllGameObjects();
        std::shared_ptr<GameObject> AddGameObject();
        std::shared_ptr<GameObject> AddGameObject(const glm::vec2 & a_Position, const glm::vec2 & a_Size);
        std::shared_ptr<GameObject> AddGameObject(const glm::vec3 & a_Position, const glm::vec3& a_Size);
        void RemoveGameObject(std::shared_ptr<GameObject> a_GameObject);
        void Update(float a_DeltaTime);
        void FixedUpdate(float a_TimeStep);
        const std::vector<std::shared_ptr<GameObject>> & GetGameObjects() const { return m_GameObjects; }
        void Save();
        void Save(const std::string& a_FilePath);
        void DestroyGameObjectImmediate(std::shared_ptr<GameObject> a_GameObject);
        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& a_Name) { m_Name = a_Name; }
        std::shared_ptr<GameObject> GetEditorCamera() const;
        void DestroyEditorCamera() const;
        std::shared_ptr<GameObject> CreateEditorCamera();
        template<typename T>
        std::shared_ptr<GameObject> FindGameObjectOfType();
        template<typename T>
        const std::vector<std::shared_ptr<GameObject>>& FindGameObjectsOfType();

        void SetAmbientLight(std::shared_ptr<AmbientLightSource> a_AmbientLightSource) { m_AmbientLight = a_AmbientLightSource; }
        glm::vec3 GetAmbientLight() const;
        const std::vector<std::shared_ptr<LightSource>>& GetLightSources() const { return m_Lights; }
        void AddLightSource(std::shared_ptr<LightSource> a_LightSource) { m_Lights.push_back(a_LightSource); }
        const glm::vec3& GetBackgroundColor() const { return m_BackgroundColor; }
        void SetBackgroundColor(const glm::vec3& a_BackgroundColor) { m_BackgroundColor = a_BackgroundColor; }
        void SetBVH(std::shared_ptr<BVH> a_BVH) { m_BVH = a_BVH; }
        std::shared_ptr<BVH> GetBVH() const { return m_BVH; }
        void ConstructBVH();

        bool RayCast(const Ray& a_Ray, float a_MaxDistance = INFINITY) const;
        bool RayCast(std::vector<std::shared_ptr<RenderComponent>>& a_RenderComponent, const Ray& a_Ray, float a_MaxDistance = INFINITY) const;
        bool RayCast(std::vector<RayHitData>& a_RayHitDatas, const Ray& a_Ray, float a_MaxDistance = INFINITY) const;
        static bool RayCast(std::vector<RayHitData>& a_RayHitDatas, const std::vector<std::shared_ptr<RenderComponent>>& a_RenderComponent, const Ray& a_Ray, float a_MaxDistance = INFINITY);

        void IncreaseRaysSend() { m_RaySend++; }
        int GetRaysSend() const { return m_RaySend;  }
        void ResetRaysSend() { m_RaySend = 0; }
        void IncreaseRaysHit() { m_RaysHit++;  }
        int GetRaysHit() const { return m_RaysHit; }
        void ResetRaysHit() { m_RaysHit = 0; }

        static std::shared_ptr<Scene> Create() { return std::make_shared<Scene>(); }
        static std::shared_ptr<Scene> Load(const std::string& a_FilePath);

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::make_nvp("Resource", cereal::base_class<Resource>(this)));
            ar(
                cereal::make_nvp("m_GameObjects", m_GameObjects)
            );
        }
    private:
        std::vector<std::shared_ptr<GameObject>> m_GameObjects;
        std::vector<std::shared_ptr<GameObject>> m_GameObjectsToAdd;
        std::vector<std::shared_ptr<GameObject>> m_GameObjectsToRemove;
        std::deque<std::string> m_States;
        const int m_MaxStateSaves = 20;
        int m_CurrentStateIndex = 0;

        int m_RaySend = 0;
        int m_RaysHit = 0;

        std::shared_ptr<AmbientLightSource> m_AmbientLight;
        std::vector<std::shared_ptr<LightSource>> m_Lights;
        std::shared_ptr<BVH> m_BVH;
        glm::vec3 m_BackgroundColor;

        void Unload();
	};

    template <typename T>
    std::shared_ptr<GameObject> Scene::FindGameObjectOfType()
    {
        MUSE_PROFILE_FUNCTION();

        std::shared_ptr<GameObject> gameObjectOfType = nullptr;

        for(auto gameObject : m_GameObjects)
        {
            if(gameObject->HasComponent<T>())
            {
                gameObjectOfType = gameObject;
            }
        }

        return gameObjectOfType;
    }

    template <typename T>
    const std::vector<std::shared_ptr<GameObject>>& Scene::FindGameObjectsOfType()
    {
        MUSE_PROFILE_FUNCTION();

        std::vector<std::shared_ptr<GameObject>> gameObjectsOfType;

        for (auto gameObject : m_GameObjects)
        {
            if (gameObject->HasComponent<T>())
            {
                gameObjectsOfType.push_back(gameObject);
            }
        }

        return gameObjectsOfType;
    }
}

CEREAL_REGISTER_TYPE_WITH_NAME(Muse::Scene, "Scene")
CEREAL_REGISTER_POLYMORPHIC_RELATION(Muse::Resource, Muse::Scene)
