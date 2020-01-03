#include "MusePCH.h"

#include "Core/System/Scene/Scene.h"

#include "Core/Application.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/TransformComponent.h"
#include "Core/System/SceneManager.h"
#include "Core/System/Manager/SystemManager.h"
#include "Core/Gameplay/Component/CameraComponent.h"
#include "Core/Utilities/json/from_json.h"
#include "Core/Utilities/json/to_json.h"

#include <rttr/registration>

namespace Muse
{
    Scene::Scene()
    {
        CreateEditorCamera();
    }

    Scene::~Scene()
    {
        Unload();
        _ASSERT(m_GameObjectsToUpdate.size() == 0);
    }

    void Scene::Unload()
    {
        DestroyAllGameObjects();
    }

    void Scene::DestroyAllGameObjects()
    {
        for (GameObject* gameObject : m_GameObjectsToUpdate)
        {
            delete gameObject;
        }
        m_GameObjectsToUpdate.clear();

        for (GameObject* gameObject : m_GameObjectsToAdd)
        {
            delete gameObject;
        }
        m_GameObjectsToAdd.clear();

        for (GameObject* gameObject : m_GameObjectsToRemove)
        {
            delete gameObject;
        }
        m_GameObjectsToRemove.clear();
    }

    GameObject& Scene::AddGameObject()
    {
        GameObject* gameObject = new GameObject();
        gameObject->Init(*this);
        m_GameObjectsToAdd.push_back(gameObject);
        return *gameObject;
    }

    GameObject& Scene::AddGameObject(const glm::vec2& a_Position, const glm::vec2& a_Size)
    {
        GameObject& gameObject = AddGameObject();
        gameObject.GetTransform()->SetPosition(a_Position);
        gameObject.GetTransform()->SetScale(a_Size);
        return gameObject;
    }

    GameObject& Scene::AddGameObject(const glm::vec3& a_Position, const glm::vec3& a_Size)
    {
        GameObject& gameObject = AddGameObject();
        gameObject.GetTransform()->SetPosition(a_Position);
        gameObject.GetTransform()->SetScale(a_Size);
        return gameObject;
    }

    void Scene::RemoveGameObject(GameObject& a_GameObject)
    {
        ASSERT(std::find(m_GameObjectsToRemove.begin(), m_GameObjectsToRemove.end(), &a_GameObject) == m_GameObjectsToRemove.end(), "GameObject to remove does not exists!");
        m_GameObjectsToRemove.push_back(&a_GameObject);
    }

    void Scene::Update(float a_DeltaTime)
    {
        for (GameObject* gameObject : m_GameObjectsToAdd)
        {
            m_GameObjectsToUpdate.push_back(gameObject);
        }
        m_GameObjectsToAdd.clear();

        for (GameObject* gameObject : m_GameObjectsToUpdate)
        {
            gameObject->Update(a_DeltaTime);
        }

        for (GameObject* gameObjectToRemove : m_GameObjectsToRemove)
        {
            DestroyGameObjectImmediate(gameObjectToRemove);
        }
        m_GameObjectsToRemove.clear();
    }

    void Scene::FixedUpdate(float a_TimeStep)
    {
        for (GameObject* gameObject : m_GameObjectsToUpdate)
        {
            gameObject->FixedUpdate(a_TimeStep);
        }
    }

    void Scene::Deserialize(const std::string& a_Json)
    {
        Unload();

        io::from_json(a_Json, *this);

        for (GameObject* gameObject : m_GameObjectsToUpdate)
        {
            gameObject->Init(*this);

            for (Component* component : gameObject->GetComponents())
            {
                component->Init(gameObject);
            }
        }
    }

    std::string Scene::Serialize() const
    {
        return io::to_json(*this);
    }

    void Scene::Save()
    {
        Save(m_Path);
    }

    void Scene::Save(const std::string& a_Path)
    {
        std::experimental::filesystem::path path{ a_Path }; //creates TestingFolder object on C:
        std::experimental::filesystem::create_directories(path.parent_path()); //add directories based on the object path (without this line it will not work)

        DestroyEditorCamera();

        std::string jsonString = Serialize();

        CreateEditorCamera();

        std::ofstream ofs(path);
        ofs << jsonString;
        ofs.close();
    }

    void Scene::Load(const std::string& a_Path)
    {
    #ifdef MUSE_DEBUG 
        if (!std::experimental::filesystem::exists(a_Path))
        {
            LOG_ENGINE_ERROR("Scene does not exists! {0}", a_Path);
            ASSERT_ENGINE(false, "Scene does not exists!");
        }
    #endif

        std::ifstream stream(a_Path);
        std::string sceneJSON((std::istreambuf_iterator<char>(stream)),
                              std::istreambuf_iterator<char>());

        Deserialize(sceneJSON);
    }

    void Scene::SaveState()
    {
        _ASSERT(m_MaxStateSaves > 0);

        while (m_CurrentStateIndex < m_States.size())
        {
            m_States.pop_back(); // Remove any undo's from stack if a change has been made
        }

        DestroyEditorCamera();

        std::string serialized = Serialize();
        m_States.push_back(serialized);

        CreateEditorCamera();

        ASSERT_ENGINE(m_States.size() <= m_MaxStateSaves + 1, "m_States size is over the m_UndoCount");

        if (m_States.size() > m_MaxStateSaves)
        {
            m_States.pop_front();
        }

        m_CurrentStateIndex = static_cast<int>(m_States.size());

        LOG_ENGINE_INFO("m_CurrentStateIndex: {0}", m_CurrentStateIndex);
    }

    void Scene::DestroyGameObjectImmediate(GameObject* a_GameObject)
    {
        m_GameObjectsToAdd.erase(std::remove(m_GameObjectsToAdd.begin(), m_GameObjectsToAdd.end(), a_GameObject), m_GameObjectsToAdd.end());
        m_GameObjectsToUpdate.erase(std::remove(m_GameObjectsToUpdate.begin(), m_GameObjectsToUpdate.end(), a_GameObject), m_GameObjectsToUpdate.end());

        delete a_GameObject;
    }

    void Scene::Undo()
    {
        if (CanUndo())
        {
            m_CurrentStateIndex--;
            std::string serialized = m_States.at(m_CurrentStateIndex);
            Deserialize(serialized);

            LOG_ENGINE_INFO("m_CurrentStateIndex: {0}", m_CurrentStateIndex);
        }
    }

    void Scene::Redo()
    {
        if (m_States.size() > 0
            && m_CurrentStateIndex < m_States.size() - 1)
        {
            m_CurrentStateIndex++;
            std::string serialized = m_States.at(m_CurrentStateIndex);
            Deserialize(serialized);

            LOG_ENGINE_INFO("m_CurrentStateIndex: {0}", m_CurrentStateIndex);
        }
    }

    GameObject* Scene::GetEditorCamera() const
    {
        GameObject* editorCameraGameObject = nullptr;
        for (GameObject* gameObject : m_GameObjectsToUpdate)
        {
            CameraComponent* camera = gameObject->GetComponent<CameraComponent>();

            if (camera != nullptr
                && camera->IsEditorCamera())
            {
                editorCameraGameObject = gameObject;
                break;
            }
        }

        return editorCameraGameObject;
    }

    void Scene::DestroyEditorCamera()
    {
        GameObject* cameraGameObject = GetEditorCamera();
        ASSERT(cameraGameObject != nullptr, "EditorCamera does not exists!");
        delete cameraGameObject;
    }

    GameObject& Scene::CreateEditorCamera()
    {
        ASSERT(GetEditorCamera() == nullptr, "EditorCamera already exists!");
        GameObject& gameObject = AddGameObject();

        gameObject.AddComponent<CameraComponent>().MakeEditorCamera();

        return gameObject;
    }

    //#endif

    RTTR_REGISTRATION
    {
        rttr::registration::class_<Scene>("Scene")
            .constructor<>()
            (
                rttr::policy::ctor::as_raw_ptr
            )
            .property("Name", &Scene::GetName, &Scene::SetName)
            .property("GameObjects", &Scene::GetGameObjects, &Scene::SetGameObjects)
        ;
    }
}