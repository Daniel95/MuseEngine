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
#include "Core/Gameplay/GameObject.h"

namespace Muse 
{
    class Component;
    class GameObject;
    class TransformComponent;
    class CameraComponent;
    class Application;

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
        const std::vector<std::shared_ptr<GameObject>> & GetGameObjects() const { return m_GameObjectsToUpdate; }
        //void Deserialize(const std::string& a_Json);
        //std::string Serialize() const;
        void Save();
        void Save(const std::string& a_FilePath);
        //void Load();
        //void Load(const std::string& a_Path);
        //void SaveState();
        void DestroyGameObjectImmediate(std::shared_ptr<GameObject> a_GameObject);
        //bool CanUndo() const { return m_States.size() > 0 && m_CurrentStateIndex > 0; }
        //bool CanRedo() const { return m_States.size() > 0 && m_CurrentStateIndex < m_States.size() - 1; }
        //void Undo();
        //void Redo();
        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& a_Name) { m_Name = a_Name; }
        std::shared_ptr<GameObject> GetEditorCamera() const;
        void DestroyEditorCamera();
        std::shared_ptr<GameObject> CreateEditorCamera();
        template<typename T>
        std::shared_ptr<GameObject> FindGameObjectOfType();
        template<typename T>
        const std::vector<std::shared_ptr<GameObject>>& FindGameObjectsOfType();

        static std::shared_ptr<Scene> Create() { return std::make_shared<Scene>(); }
        static std::shared_ptr<Scene> Load(const std::string& a_FilePath);

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::make_nvp("Resource", cereal::base_class<Resource>(this)));
            ar(
                cereal::make_nvp("m_GameObjectsToUpdate", m_GameObjectsToUpdate),
                cereal::make_nvp("m_GameObjectsToAdd", m_GameObjectsToAdd),
                cereal::make_nvp("m_GameObjectsToRemove", m_GameObjectsToRemove)
            );
        }
    private:
        std::vector<std::shared_ptr<GameObject>> m_GameObjectsToUpdate;
        std::vector<std::shared_ptr<GameObject>> m_GameObjectsToAdd;
        std::vector<std::shared_ptr<GameObject>> m_GameObjectsToRemove;
        std::deque<std::string> m_States;
        const int m_MaxStateSaves = 20;
        int m_CurrentStateIndex = 0;

        void Unload();
	};

    template <typename T>
    std::shared_ptr<GameObject> Scene::FindGameObjectOfType()
    {
        MUSE_PROFILE_FUNCTION();

        std::shared_ptr<GameObject> gameObjectOfType = nullptr;

        for(auto gameObject : m_GameObjectsToUpdate)
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

        for (auto gameObject : m_GameObjectsToUpdate)
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
