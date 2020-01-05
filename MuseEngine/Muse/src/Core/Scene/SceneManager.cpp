#include "MusePCH.h"

#include "Core/Scene/SceneManager.h"
#include "Core/Application.h"
#include "Core/Resource//ResourceManager.h"
#include "Core/Scene/Scene.h"
#include "Core/Utilities/Log.h"
#include "Core/Utilities/Defines.h"
#include "Core/Window.h"

namespace Muse 
{
    std::string SceneManager::m_SceneNameToLoad = "";
    std::shared_ptr<Scene> SceneManager::m_ActiveScene = nullptr;
    bool SceneManager::m_InspectLoadedScenes = false;

    SceneManager::SceneManager()
    {
        MUSE_PROFILE_FUNCTION();

        Application::Get().m_UpdateEvent.Subscribe(SUB_FN(SceneManager::OnUpdate, std::placeholders::_1));
    }

    SceneManager::~SceneManager()
    {
        MUSE_PROFILE_FUNCTION();

        Application::Get().m_UpdateEvent.Unsubscribe(this);
    }

    void SceneManager::OnUpdate(float a_DeltaTime)
    {
        MUSE_PROFILE_FUNCTION();

        if (m_SceneNameToLoad != "")
        {
            LoadSceneImmediate(m_SceneNameToLoad);
            m_SceneNameToLoad = "";
        }

        if (m_ActiveScene != nullptr)
        {
            m_ActiveScene->Update(a_DeltaTime);
        }
    }

    std::shared_ptr<Scene> SceneManager::NewScene()
    {
        MUSE_PROFILE_FUNCTION();

        if (m_ActiveScene != nullptr)
        {
            const std::string oldScenePath = GAME_SCENE_PATH + m_ActiveScene->GetName() + ".txt";
            ResourceManager::UnloadResource<Scene>(oldScenePath);
        }

        const std::string newScenePath = GAME_SCENE_PATH + "NewScene.txt";
        m_ActiveScene = std::make_shared<Scene>();

        return m_ActiveScene;
    }

    void SceneManager::LoadScene(const std::string& a_SceneName)
    {
        MUSE_PROFILE_FUNCTION();

        m_SceneNameToLoad = a_SceneName;
    }

    void SceneManager::ReloadScene()
    {
        MUSE_PROFILE_FUNCTION();

        ASSERT_ENGINE(m_ActiveScene != nullptr, "There is not active scene to reload!");

        m_SceneNameToLoad = m_ActiveScene->GetName();
    }

    std::shared_ptr<Scene> SceneManager::GetActiveScene()
    {
        MUSE_PROFILE_FUNCTION();

        return m_ActiveScene;
    }

    void SceneManager::DestroyAllGameObjects()
    {
        MUSE_PROFILE_FUNCTION();

        if(m_ActiveScene != nullptr)
        {
            m_ActiveScene->DestroyAllGameObjects();
        }
    }

    void SceneManager::LoadSceneImmediate(const std::string& a_SceneName)
    {
        MUSE_PROFILE_FUNCTION();

        if (m_ActiveScene != nullptr)
        {
            const std::string oldScenePath = GAME_SCENE_PATH + m_ActiveScene->GetName() + ".txt";
            ResourceManager::UnloadResource<Scene>(oldScenePath);
        }

        const std::string newScenePath = GAME_SCENE_PATH + a_SceneName + ".txt";
        m_ActiveScene = ResourceManager::Load<Scene>(newScenePath);
        m_ActiveScene->Load(newScenePath);
    }
}
