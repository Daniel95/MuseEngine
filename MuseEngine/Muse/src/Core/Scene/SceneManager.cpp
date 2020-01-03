#include "MusePCH.h"

#include "Core/Scene/SceneManager.h"

#include "Core/Application.h"
#include "Core/Resource//ResourceManager.h"
#include "Core/Scene/Scene.h"
#include "Core/Utilities/Log.h"
#include "Core/Window.h"

namespace Muse 
{
    std::string SceneManager::m_SceneNameToLoad = "";
    std::shared_ptr<Scene> SceneManager::m_ActiveScene = nullptr;
    bool SceneManager::m_InspectLoadedScenes = false;

    SceneManager::SceneManager()
    {
        Application::Get().m_UpdateEvent.Subscribe(this, std::bind(&SceneManager::OnUpdate, this, std::placeholders::_1));
    }

    SceneManager::~SceneManager()
    {
        Application::Get().m_UpdateEvent.Unsubscribe(this);
    }

    void SceneManager::OnUpdate(float a_DeltaTime)
    {
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
        if (m_ActiveScene != nullptr)
        {
            const std::string oldScenePath = GAME_SCENE_PATH + m_ActiveScene->GetName() + ".txt";
            ResourceManager::UnloadResource<Scene>(oldScenePath);
        }

        const std::string newScenePath = GAME_SCENE_PATH + "NewScene.txt";
        m_ActiveScene = ResourceManager::Load<Scene>(newScenePath);

        return m_ActiveScene;
    }

    void SceneManager::LoadScene(const std::string& a_SceneName)
    {
        m_SceneNameToLoad = a_SceneName;
    }

    void SceneManager::ReloadScene()
    {
        _ASSERT(m_ActiveScene != nullptr);

        m_SceneNameToLoad = m_ActiveScene->GetName();
    }

    std::shared_ptr<Scene> SceneManager::GetActiveScene()
    {
        return m_ActiveScene;
    }

    void SceneManager::LoadSceneImmediate(const std::string& a_SceneName)
    {
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
