#include "MusePCH.h"

#include "Core/Scene/Scene.h"
#include "Core/Application.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/TransformComponent.h"
#include "Core/Gameplay/Component/CameraComponent.h"
#include "Core/Utilities/json/from_json.h"
#include "Core/Utilities/json/to_json.h"

#include <rttr/registration>
#include <filesystem>
#include "Core/Gameplay/Component/OrthographicCameraControllerComponent.h"

namespace Muse
{
    Scene::Scene()
    {
        MUSE_PROFILE_FUNCTION();

        CreateEditorCamera();
    }

    Scene::~Scene()
    {
        MUSE_PROFILE_FUNCTION();

        Unload();
        ASSERT_ENGINE(m_GameObjectsToUpdate.size() == 0, "Not all gameobjects have been destroyed!");
    }

    void Scene::Unload()
    {
        MUSE_PROFILE_FUNCTION();

        DestroyAllGameObjects();
    }

    void Scene::DestroyAllGameObjects()
    {
        MUSE_PROFILE_FUNCTION();

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
        MUSE_PROFILE_FUNCTION();

        GameObject* gameObject = new GameObject();
        gameObject->Init(*this);
        m_GameObjectsToAdd.push_back(gameObject);
        return *gameObject;
    }

    GameObject& Scene::AddGameObject(const glm::vec2& a_Position, const glm::vec2& a_Size)
    {
        MUSE_PROFILE_FUNCTION();

        GameObject& gameObject = AddGameObject();
        gameObject.GetTransform()->SetPosition(a_Position);
        gameObject.GetTransform()->SetScale(a_Size);
        return gameObject;
    }

    GameObject& Scene::AddGameObject(const glm::vec3& a_Position, const glm::vec3& a_Size)
    {
        MUSE_PROFILE_FUNCTION();

        GameObject& gameObject = AddGameObject();
        gameObject.GetTransform()->SetPosition(a_Position);
        gameObject.GetTransform()->SetScale(a_Size);
        return gameObject;
    }

    void Scene::RemoveGameObject(GameObject& a_GameObject)
    {
        MUSE_PROFILE_FUNCTION();

        ASSERT_ENGINE(std::find(m_GameObjectsToRemove.begin(), m_GameObjectsToRemove.end(), &a_GameObject) == m_GameObjectsToRemove.end(), "GameObject to remove does not exists!");
        m_GameObjectsToRemove.push_back(&a_GameObject);
    }

    void Scene::Update(float a_DeltaTime)
    {
        MUSE_PROFILE_FUNCTION();

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
        MUSE_PROFILE_FUNCTION();

        for (GameObject* gameObject : m_GameObjectsToUpdate)
        {
            gameObject->FixedUpdate(a_TimeStep);
        }
    }

    void Scene::Deserialize(const std::string& a_Json)
    {
        MUSE_PROFILE_FUNCTION();

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
        MUSE_PROFILE_FUNCTION();

        Save(m_Path);
    }

    void Scene::Save(const std::string& a_Path)
    {
        MUSE_PROFILE_FUNCTION();

        std::filesystem::path path{ a_Path }; //creates TestingFolder object on C:
        std::filesystem::create_directories(path.parent_path()); //add directories based on the object path (without this line it will not work)

        DestroyEditorCamera();

        std::string jsonString = Serialize();;

        CreateEditorCamera();

        std::ofstream ofs(path);
        ofs << jsonString;
        ofs.close();
    }

    /*
    void Scene::Load(const std::string& a_Path)
    {
        MUSE_PROFILE_FUNCTION();

    #ifdef MUSE_DEBUG 
        if (!std::filesystem::exists(a_Path))
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
    */

    void Scene::SaveState()
    {
        MUSE_PROFILE_FUNCTION();

        ASSERT_ENGINE(m_MaxStateSaves > 0, "Max save states cannot be zero!");

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
        MUSE_PROFILE_FUNCTION();

        m_GameObjectsToAdd.erase(std::remove(m_GameObjectsToAdd.begin(), m_GameObjectsToAdd.end(), a_GameObject), m_GameObjectsToAdd.end());
        m_GameObjectsToUpdate.erase(std::remove(m_GameObjectsToUpdate.begin(), m_GameObjectsToUpdate.end(), a_GameObject), m_GameObjectsToUpdate.end());

        delete a_GameObject;
    }

    void Scene::Undo()
    {
        MUSE_PROFILE_FUNCTION();

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
        MUSE_PROFILE_FUNCTION();

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
        MUSE_PROFILE_FUNCTION();

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
        MUSE_PROFILE_FUNCTION();

        GameObject* cameraGameObject = GetEditorCamera();
        ASSERT_ENGINE(cameraGameObject != nullptr, "EditorCamera does not exists!");
        delete cameraGameObject;
    }

    GameObject& Scene::CreateEditorCamera()
    {
        MUSE_PROFILE_FUNCTION();

        ASSERT_ENGINE(GetEditorCamera() == nullptr, "EditorCamera already exists!");
        GameObject& gameObject = AddGameObject();

        gameObject.AddComponent<CameraComponent>().MakeEditorCamera();
        gameObject.AddComponent<OrthographicCameraControllerComponent>();

        return gameObject;
    }

    std::shared_ptr<Scene> Scene::Load(const std::string& a_FilePath)
    {
        MUSE_PROFILE_FUNCTION();

    #ifdef MUSE_DEBUG 
        if (!std::filesystem::exists(a_FilePath))
        {
            LOG_ENGINE_ERROR("Scene does not exists! {0}", a_FilePath);
            ASSERT_ENGINE(false, "Scene does not exists!");
        }
    #endif

        std::ifstream stream(a_FilePath);
        std::string sceneJSON((std::istreambuf_iterator<char>(stream)),
            std::istreambuf_iterator<char>());

        std::shared_ptr<Scene> scene = std::make_shared<Scene>();

        io::from_json(sceneJSON, scene);

        for (GameObject* gameObject : scene->GetGameObjects())
        {
            gameObject->Init(*scene);

            for (Component* component : gameObject->GetComponents())
            {
                component->Init(gameObject);
            }
        }

        return scene;
    }

    //#endif

    RTTR_REGISTRATION
    {
        rttr::registration::class_<Scene>("Scene")
            .constructor<>()
            (
                rttr::policy::ctor::as_raw_ptr
            )
            .property("GameObjects", &Scene::GetGameObjects, &Scene::SetGameObjects);
    }
}
