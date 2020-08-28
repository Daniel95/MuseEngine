#include "MusePCH.h"

#include "Core/Scene/SceneOld.h"
#include "Core/Application.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/TransformComponentOld.h"
#include "Core/Gameplay/Component/CameraComponentOld.h"
#include "Core/Gameplay/Component/Component.h"

#include "Core/Utilities/json/from_json.h"
#include "Core/Utilities/json/to_json.h"
#include <rttr/registration>

#include <filesystem>
#include "Core/Gameplay/Component/OrthographicCameraControllerComponent.h"
#include "Core/Input/Input.h"
#include "Core/Input/KeyCodes.h"

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/cereal.hpp>
#include "Core/Renderer/RayTracer/AmbientLightSource.h"
#include "Core/Renderer/RayTracer/BVH/BVH.h"

#include "Core/Renderer/RayTracer/AmbientLightSource.h"
#include "Core/Renderer/RayTracer/BVH/BVH.h"
#include "Core/Renderer/RayTracer/Ray.h"
#include "Core/Renderer/RayTracer/AmbientLightSource.h"
#include "Core/Gameplay/Component/RenderComponent.h"
#include "Core/Renderer/RayTracer/RayHitData.h"
#include "Core/Gameplay/Component/PerspectiveCameraControllerComponent.h"

namespace Muse
{
    SceneOld::SceneOld()
    {
        MUSE_PROFILE_FUNCTION();

        CreateEditorCamera();
    }

    SceneOld::~SceneOld()
    {
        MUSE_PROFILE_FUNCTION();

        Unload();
        ASSERT_ENGINE(m_GameObjects.size() == 0, "Not all gameobjects have been destroyed!");
    }

    void SceneOld::Unload()
    {
        MUSE_PROFILE_FUNCTION();

        DestroyAllGameObjects();
    }

    void SceneOld::DestroyAllGameObjects()
    {
        MUSE_PROFILE_FUNCTION();

        for (std::shared_ptr<GameObject> gameObject : m_GameObjects)
        {
            gameObject.reset();
        }
        m_GameObjects.clear();

        for (std::shared_ptr<GameObject> gameObject : m_GameObjectsToAdd)
        {
            gameObject.reset();
        }
        m_GameObjectsToAdd.clear();

        for (std::shared_ptr<GameObject> gameObject : m_GameObjectsToRemove)
        {
            gameObject.reset();
        }
        m_GameObjectsToRemove.clear();
    }

    std::shared_ptr<GameObject> SceneOld::AddGameObject()
    {
        MUSE_PROFILE_FUNCTION();

        std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();
        gameObject->Init(*this);
        m_GameObjectsToAdd.push_back(gameObject);
        return gameObject;
    }

    std::shared_ptr<GameObject> SceneOld::AddGameObject(const glm::vec2& a_Position, const glm::vec2& a_Size)
    {
        MUSE_PROFILE_FUNCTION();

        std::shared_ptr<GameObject> gameObject = AddGameObject();
        gameObject->GetTransform()->SetLocalPosition(a_Position);
        gameObject->GetTransform()->SetLocalScale(a_Size);
        return gameObject;
    }

    std::shared_ptr<GameObject> SceneOld::AddGameObject(const glm::vec3& a_Position, const glm::vec3& a_Size)
    {
        MUSE_PROFILE_FUNCTION();

        std::shared_ptr<GameObject> gameObject = AddGameObject();
        gameObject->GetTransform()->SetLocalPosition(a_Position);
        gameObject->GetTransform()->SetLocalScale(a_Size);
        return gameObject;
    }

    void SceneOld::RemoveGameObject(std::shared_ptr<GameObject> a_GameObject)
    {
        MUSE_PROFILE_FUNCTION();

        ASSERT_ENGINE(std::find(m_GameObjectsToRemove.begin(), m_GameObjectsToRemove.end(), a_GameObject) == m_GameObjectsToRemove.end(), "GameObject to remove does not exists!");
        m_GameObjectsToRemove.push_back(a_GameObject);
    }

    void SceneOld::Update(float a_DeltaTime)
    {
        MUSE_PROFILE_FUNCTION();

        for (auto gameObject : m_GameObjectsToAdd)
        {
            m_GameObjects.push_back(gameObject);
        }
        m_GameObjectsToAdd.clear();

        for (auto gameObject : m_GameObjects)
        {
            gameObject->Update(a_DeltaTime);
        }

        for (auto gameObjectToRemove : m_GameObjectsToRemove)
        {
            DestroyGameObjectImmediate(gameObjectToRemove);
        }
        m_GameObjectsToRemove.clear();

        /*
        if (Editor::GetEditorMode())
        {
            bool ctrlS = Input::GetKeyDown(MUSE_KEY_LEFT_CONTROL) && Input::GetKeyDown(MUSE_KEY_S);

            if(ctrlS)
            {
                LOG_ENGINE_INFO("SAVE");

                const std::string path = Editor::GetSavePath("txt");
                Save(path);
            }
        }
        */
    }

    void SceneOld::FixedUpdate(float a_TimeStep)
    {
        MUSE_PROFILE_FUNCTION();

        for (const auto gameObject : m_GameObjects)
        {
            gameObject->FixedUpdate(a_TimeStep);
        }
    }

    void SceneOld::Save()
    {
        Save(m_Path);
    }

    void SceneOld::Save(const std::string& a_FilePath)
    {
        MUSE_PROFILE_FUNCTION();

        for (auto gameObject : m_GameObjectsToAdd)
        {
            m_GameObjects.push_back(gameObject);
        }
        m_GameObjectsToAdd.clear();

        for (auto gameObjectToRemove : m_GameObjectsToRemove)
        {
            DestroyGameObjectImmediate(gameObjectToRemove);
        }
        m_GameObjectsToRemove.clear();


        std::string txtExtension = ".txt";
        std::string savePath = a_FilePath;

        if(a_FilePath.find(txtExtension) == std::string::npos)
        {
            savePath += txtExtension;
        }
        
        UpdatePath(savePath);
        std::filesystem::path path{ savePath }; //creates TestingFolder object on C:
        std::filesystem::create_directories(path.parent_path()); //add directories based on the object path (without this line it will not work)
        std::ofstream ofs(path);

        {
            cereal::JSONOutputArchive oarchive(ofs);
            oarchive(cereal::make_nvp(savePath, *this));
        }
        ofs.close();

        //DestroyEditorCamera();
    }

    void SceneOld::DestroyGameObjectImmediate(const std::shared_ptr<GameObject> a_GameObject)
    {
        MUSE_PROFILE_FUNCTION();

        m_GameObjectsToAdd.erase(std::remove(m_GameObjectsToAdd.begin(), m_GameObjectsToAdd.end(), a_GameObject), m_GameObjectsToAdd.end());
        m_GameObjects.erase(std::remove(m_GameObjects.begin(), m_GameObjects.end(), a_GameObject), m_GameObjects.end());
    }

    std::shared_ptr<GameObject> SceneOld::GetEditorCamera() const
    {
        MUSE_PROFILE_FUNCTION();

        std::shared_ptr<GameObject> editorCameraGameObject = nullptr;
        for (const auto gameObject : m_GameObjects)
        {
            std::shared_ptr<CameraComponentOld> camera = gameObject->GetComponent<CameraComponentOld>();

            if (camera != nullptr
                && camera->IsEditorCamera())
            {
                editorCameraGameObject = gameObject;
                break;
            }
        }

        return editorCameraGameObject;
    }

    void SceneOld::DestroyEditorCamera() const
    {
        MUSE_PROFILE_FUNCTION();

        std::shared_ptr<GameObject> cameraGameObject = GetEditorCamera();
        ASSERT_ENGINE(cameraGameObject != nullptr, "EditorCamera does not exists!");

        cameraGameObject.reset();
    }

    std::shared_ptr<GameObject> SceneOld::CreateEditorCamera()
    {
        MUSE_PROFILE_FUNCTION();

        ASSERT_ENGINE(GetEditorCamera() == nullptr, "EditorCamera already exists!");
        std::shared_ptr<GameObject> gameObject = AddGameObject();

        gameObject->AddComponent<CameraComponentOld>()->MakeEditorCamera();
        //gameObject->AddComponent<PerspectiveCameraControllerComponent>();
        //gameObject->AddComponent<OrthographicCameraControllerComponent>();

        return gameObject;
    }

    glm::vec3 SceneOld::GetAmbientLight() const
    {
        return m_AmbientLight->GetLight(glm::vec3(0));
    }

    void SceneOld::ConstructBVH()
    {
        ASSERT_ENGINE(m_BVH != nullptr, "Not BVH set!");
        m_BVH->ConstructHierarchy(RenderComponent::GetAll());
        m_BVH->PrintHierarchy();
    }

    std::shared_ptr<SceneOld> SceneOld::Load(const std::string& a_FilePath)
    {
        std::filesystem::path path{ a_FilePath }; //creates TestingFolder object on C:
        std::filesystem::create_directories(path.parent_path()); //add directories based on the object path (without this line it will not work)
        std::ifstream fs(path);

        std::shared_ptr<SceneOld> scene = std::make_shared<SceneOld>();

        {
            cereal::JSONInputArchive iarchive(fs);
            iarchive(cereal::make_nvp(a_FilePath, *scene.get()));
        }
        fs.close();

        for (auto gameObject : scene->GetGameObjects())
        {
            gameObject->Init(*scene);

            for (const auto component : gameObject->GetComponents())
            {
                component->Init(gameObject);
            }
        }

        return scene;
    }
}
