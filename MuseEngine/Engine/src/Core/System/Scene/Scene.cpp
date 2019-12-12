#include "OatPCH.h"

#include "Core/System/Scene/Scene.h"

#include "Core/Application.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/BoxCollider2D.h"
#include "Core/Gameplay/Component/Transform.h"
#include "Core/System/SceneSystem.h"
#include "Core/System/Manager/SystemManager.h"
#include "Core/Utilities/json/from_json.h"
#include "Core/Utilities/json/to_json.h"

#include <rttr/registration>

namespace Muse
{
    Scene::Scene(ullong a_ID, const std::string& a_Path)
        : Resource(a_ID, a_Path)
    {
        m_Name = a_Path;
        Replace(m_Name, GAME_SCENE_PATH, "");
        Replace(m_Name, ".txt", "");
    }

    Scene::~Scene()
    {
        Unload();
        _ASSERT(m_GameObjectsToUpdate.size() == 0);
    }

    void Scene::Init(Application& a_Application)
    {
        m_Application = &a_Application;

        SceneSystem& sceneSystem = m_Application->GetSystemManager()->GetSystem<SceneSystem>();
    }

    void Scene::SetApplication(Application& a_Application)
    {
        m_Application = &a_Application;

        // Here temporarily, to make it work
        SceneSystem& sceneSystem = m_Application->GetSystemManager()->GetSystem<SceneSystem>();
    }

    void Scene::Unload()
    {
        SceneSystem& sceneSystem = m_Application->GetSystemManager()->GetSystem<SceneSystem>();

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
        //gameObject.GetTransform()->SetPosition(a_Position);
        //gameObject.GetTransform()->SetScale(a_Size);
        return gameObject;
    }

    GameObject& Scene::AddGameObject(const glm::vec3& a_Position, const glm::vec3& a_Size)
    {
        GameObject& gameObject = AddGameObject();
        //gameObject.GetTransform()->SetPosition(a_Position);
        //gameObject.GetTransform()->SetScale(a_Size);
        return gameObject;
    }

    void Scene::RemoveGameObject(GameObject& a_GameObject)
    {
        _ASSERT(std::find(m_GameObjectsToRemove.begin(), m_GameObjectsToRemove.end(), &a_GameObject) == m_GameObjectsToRemove.end());
        m_GameObjectsToRemove.push_back(&a_GameObject);
    }

    void Scene::Update(float a_DeltaTime)
    {
        for (GameObject* gameObject : m_GameObjectsToAdd)
        {
            m_GameObjectsToUpdate.push_back(gameObject);
        }
        m_GameObjectsToAdd.clear();

    #ifndef EDITOR
        for (GameObject* gameObject : m_GameObjectsToUpdate)
        {
            gameObject->Update(a_DeltaTime);
        }
    #else 
        for (GameObject* gameObject : m_GameObjectsToUpdate)
        {
            if (gameObject->HasComponent<SpriteRenderer>())
            {
                gameObject->GetComponent<SpriteRenderer>()->Update(a_DeltaTime);
            }
        }

        EditorInteractions(a_DeltaTime);
    #endif

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

    Application* Scene::GetApplication() const
    {
        return m_Application;
    }

    const std::vector<GameObject*>& Scene::GetGameObjects()
    {
        return m_GameObjectsToUpdate;
    }

    void Scene::SetGameObjects(const std::vector<GameObject*>& a_GameObjects)
    {
        m_GameObjectsToUpdate = a_GameObjects;
    }

    void Scene::Deserialize(const std::string& a_json)
    {
        Unload();

        io::from_json(a_json, *this);

        for (GameObject* gameObject : m_GameObjectsToUpdate)
        {
            gameObject->Init(*this);

            for (Component* component : gameObject->GetComponents())
            {
                component->Init(gameObject);
            }
        }

        SceneSystem& sceneSystem = m_Application->GetSystemManager()->GetSystem<SceneSystem>();
    }

    std::string Scene::Serialize()
    {
        return io::to_json(*this);
    }

    void Scene::Save(const std::string& a_Path, const std::string& a_Name)
    {
        const std::string fullPath = a_Path + a_Name + ".txt";
        std::experimental::filesystem::path path{ fullPath }; //creates TestingFolder object on C:
        std::experimental::filesystem::create_directories(path.parent_path()); //add directories based on the object path (without this line it will not work)

    #ifdef EDITOR
        DestroyEditorCamera();

        GameObject* objectWithCam = nullptr;
        for (GameObject* gameObject : m_GameObjectsToUpdate)
        {
            if (gameObject->GetComponent<Camera>() != nullptr)
            {
                objectWithCam = gameObject;
                gameObject->Enable();
                break;
            }
        }
    #endif // EDITOR

        std::string jsonString = Serialize();

    #ifdef EDITOR
        if (objectWithCam != nullptr)
        {
            objectWithCam->Disable();
        }

        CreateEditorCamera();
    #endif // EDITOR

        std::ofstream ofs(path);
        ofs << jsonString;
        ofs.close();
    }

    void Scene::Load(const std::string& a_Path, const std::string& a_Name)
    {
        const std::string fullPath = a_Path + a_Name + ".txt";

        if (!std::experimental::filesystem::exists(fullPath))
        {
            LOG_ENGINE_ERROR("Scene does not exists! {0}", fullPath);
            _ASSERT(false);
        }

        std::ifstream stream(fullPath);
        std::string sceneJSON((std::istreambuf_iterator<char>(stream)),
                              std::istreambuf_iterator<char>());

        Deserialize(sceneJSON);
    }

    void Scene::SaveState()
    {
        _ASSERT(mc_MaxStateSaves > 0);

        while (m_CurrentStateIndex < m_States.size())
        {
            m_States.pop_back(); // Remove any undo's from stack if a change has been made
        }

    #ifdef EDITOR
        GameObject* objectWithCam = nullptr;
        if (Camera::HasMainCamera())
        {
            objectWithCam = Camera::GetMainCamera().GetGameObject();
            bool isEditorCam = objectWithCam->HasComponent<EditorCamera>();
            if (isEditorCam)
            {
                for (auto objectItr = m_GameObjectsToUpdate.begin(); objectItr != m_GameObjectsToUpdate.end(); ++objectItr)
                {
                    GameObject* object = *objectItr;
                    if (object == objectWithCam)
                    {
                        m_GameObjectsToUpdate.erase(objectItr);
                        break;
                    }
                }
            }
            else
            {
                objectWithCam = nullptr; // MainCam is not an editor cam, thus correctly created later
            }
        }
    #endif // EDITOR

        std::string serialized = Serialize();
        m_States.push_back(serialized);

    #ifdef EDITOR
        if (objectWithCam != nullptr)
        {
            m_GameObjectsToUpdate.push_back(objectWithCam);
        }
    #endif // EDITOR

        if (m_States.size() > mc_MaxStateSaves + 1)
        {
            LOG_ENGINE_ERROR("m_States size is over the m_UndoCount");
            _ASSERT(false);
        }
        else if (m_States.size() > mc_MaxStateSaves)
        {
            m_States.pop_front();
        }

        m_CurrentStateIndex = (int) m_States.size();

        LOG_ENGINE_INFO("m_CurrentStateIndex: {0}", m_CurrentStateIndex);
    }

    void Scene::DestroyGameObjectImmediate(GameObject* a_GameObject)
    {
        m_GameObjectsToAdd.erase(std::remove(m_GameObjectsToAdd.begin(), m_GameObjectsToAdd.end(), a_GameObject), m_GameObjectsToAdd.end());
        m_GameObjectsToUpdate.erase(std::remove(m_GameObjectsToUpdate.begin(), m_GameObjectsToUpdate.end(), a_GameObject), m_GameObjectsToUpdate.end());

        delete a_GameObject;
    }

    bool Scene::CanUndo()
    {
        return m_States.size() > 0 && m_CurrentStateIndex > 0;
    }

    bool Scene::CanRedo()
    {
        return m_States.size() > 0 && m_CurrentStateIndex < m_States.size() - 1;
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

    const std::string& Scene::GetName() const
    {
        return m_Name;
    }

    void Scene::SetName(const std::string& a_Name)
    {
        m_Name = a_Name;
    }

    RTTR_REGISTRATION
    {
        rttr::registration::class_<Scene>("Scene")
            .constructor<ullong, const std::string&>()
            (
                rttr::policy::ctor::as_raw_ptr
            )
            .property("Name", &Scene::GetName, &Scene::SetName)
            .property("GameObjects", &Scene::GetGameObjects, &Scene::SetGameObjects)
        ;
    }
}