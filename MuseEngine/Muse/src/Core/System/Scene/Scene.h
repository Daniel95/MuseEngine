#pragma once

#include "Core/System/Resource/Resource.h"
#include "Core/Gameplay/Component/Component.h"
#include "Core/Gameplay/Component/Transform.h"
#include "Core/Utilities/Defines.h"

#include <rttr/variant.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <deque>
#include <string>
#include <vector>

namespace Muse 
{
    class CameraComponent;
    class Application;
    class GameObject;

	class Scene : public Resource
	{
        RTTR_ENABLE(Resource);

	public:
		Scene(ullong a_ID, const std::string& a_Path);
        ~Scene();

        void DestroyAllGameObjects();
        GameObject& AddGameObject();
        GameObject& AddGameObject(const glm::vec2 & a_Position, const glm::vec2 & a_Size);
        GameObject& AddGameObject(const glm::vec3 & a_Position, const glm::vec3& a_Size);
        void RemoveGameObject(GameObject& a_GameObject);
        void Update(float a_DeltaTime);
        void FixedUpdate(float a_TimeStep);
        const std::vector<GameObject*> & GetGameObjects() const { return m_GameObjectsToUpdate; }
        void SetGameObjects(const std::vector<GameObject*> & a_GameObjects) { m_GameObjectsToUpdate = a_GameObjects; }
        void Deserialize(const std::string& a_Path);
        std::string Serialize() const;
        void Save(const std::string& a_Path, const std::string& a_Name);
		void Load(const std::string& a_Path, const std::string& a_Name);
        void SaveState();
        void DestroyGameObjectImmediate(GameObject* a_GameObject);
        bool CanUndo() const { return m_States.size() > 0 && m_CurrentStateIndex > 0; }
        bool CanRedo() const { return m_States.size() > 0 && m_CurrentStateIndex < m_States.size() - 1; }
        void Undo();
        void Redo();
        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& a_Name) { m_Name = a_Name; }
        GameObject* GetEditorCamera() const;
        void DestroyEditorCamera();
        GameObject& CreateEditorCamera();

    private:
        std::vector<GameObject*> m_GameObjectsToUpdate;
        std::vector<GameObject*> m_GameObjectsToAdd;
        std::vector<GameObject*> m_GameObjectsToRemove;
        std::deque<std::string> m_States;
        std::string m_Name;
        const int m_MaxStateSaves = 20;
        int m_CurrentStateIndex = 0;

        void Unload();

	};
}