#pragma once

#include "Core/Gameplay/Component/Component.h"
#include "Core/Gameplay/Component/TransformComponent.h"
#include "Core/Utilities/Defines.h"
#include "Core/Gameplay/GameObject.h"

#include <rttr/variant.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <deque>
#include <string>
#include <vector>
#include "Core/Resource/Resource.h"
#include "Core/Instrumentor.h"

namespace Muse 
{
    class CameraComponent;
    class Application;

	class Scene : public Resource
	{
	public:
		Scene();
        virtual ~Scene();

        void DestroyAllGameObjects();
        GameObject& AddGameObject();
        GameObject& AddGameObject(const glm::vec2 & a_Position, const glm::vec2 & a_Size);
        GameObject& AddGameObject(const glm::vec3 & a_Position, const glm::vec3& a_Size);
        void RemoveGameObject(GameObject& a_GameObject);
        void Update(float a_DeltaTime);
        void FixedUpdate(float a_TimeStep);
        const std::vector<GameObject*> & GetGameObjects() const { return m_GameObjectsToUpdate; }
        void SetGameObjects(const std::vector<GameObject*> & a_GameObjects) { m_GameObjectsToUpdate = a_GameObjects; }
        //void Deserialize(const std::string& a_Json);
        //std::string Serialize() const;
        void Save();
        void Save(const std::string& a_FilePath);
        //void Load();
        //void Load(const std::string& a_Path);
        //void SaveState();
        void DestroyGameObjectImmediate(GameObject* a_GameObject);
        //bool CanUndo() const { return m_States.size() > 0 && m_CurrentStateIndex > 0; }
        //bool CanRedo() const { return m_States.size() > 0 && m_CurrentStateIndex < m_States.size() - 1; }
        //void Undo();
        //void Redo();
        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& a_Name) { m_Name = a_Name; }
        GameObject* GetEditorCamera() const;
        void DestroyEditorCamera();
        GameObject& CreateEditorCamera();
        template<typename T>
        GameObject* FindGameObjectOfType();
        template<typename T>
        const std::vector<GameObject*>& FindGameObjectsOfType();

        static std::shared_ptr<Scene> Create() { return std::make_shared<Scene>(); }
        static std::shared_ptr<Scene> Load(const std::string& a_FilePath);

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(m_CurrentStateIndex);
        }
    private:
        std::vector<GameObject*> m_GameObjectsToUpdate;
        std::vector<GameObject*> m_GameObjectsToAdd;
        std::vector<GameObject*> m_GameObjectsToRemove;
        std::deque<std::string> m_States;
        const int m_MaxStateSaves = 20;
        int m_CurrentStateIndex = 0;

        void Unload();
	};

    template <typename T>
    GameObject* Scene::FindGameObjectOfType()
    {
        MUSE_PROFILE_FUNCTION();

        GameObject* gameObjectOfType = nullptr;

        for(GameObject* gameObject : m_GameObjectsToUpdate)
        {
            if(gameObject->HasComponent<T>())
            {
                gameObjectOfType = gameObject;
            }
        }

        return gameObjectOfType;
    }

    template <typename T>
    const std::vector<GameObject*>& Scene::FindGameObjectsOfType()
    {
        MUSE_PROFILE_FUNCTION();

        std::vector<GameObject*> gameObjectsOfType;

        for (GameObject* gameObject : m_GameObjectsToUpdate)
        {
            if (gameObject->HasComponent<T>())
            {
                gameObjectsOfType.push_back(gameObject);
            }
        }

        return gameObjectsOfType;
    }
}
