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
    std::shared_ptr<Scene> SceneManager::m_ActiveScene = nullptr;
    std::shared_ptr<Scene> SceneManager::m_SceneToSwitchTo = nullptr;

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

        if (m_SceneToSwitchTo != nullptr)
        {
            SwitchSceneImmediate(m_SceneToSwitchTo);
            m_SceneToSwitchTo = nullptr;
        }

        if (m_ActiveScene != nullptr)
        {
            m_ActiveScene->OnUpdate(a_DeltaTime);
        }
    }

    void SceneManager::SwitchScene(std::shared_ptr<Scene> a_NextScene)
    {
        if (m_ActiveScene != nullptr)
        {
            ResourceManager::UnloadResource<Scene>(m_ActiveScene->GetPath());
            m_SceneToSwitchTo = a_NextScene;
        }
        else // Immediately load the new scene when there is no active scene yet.
        {
            SwitchSceneImmediate(a_NextScene);
        }
    }

    void SceneManager::SwitchScene(const std::string& a_SceneName)
    {
        const std::shared_ptr<Scene> scene = ResourceManager::Get<Scene>(a_SceneName);

        if (m_ActiveScene != nullptr)
        {
            ResourceManager::UnloadResource<Scene>(m_ActiveScene->GetPath());
            m_SceneToSwitchTo = scene;
        }
        else // Immediately load the new scene when there is no active scene yet.
        {
            SwitchSceneImmediate(scene);
        }
    }

    void SceneManager::ReloadScene()
    {
        MUSE_PROFILE_FUNCTION();

        ASSERT_ENGINE(m_ActiveScene != nullptr, "There is not active scene to reload!");

        m_SceneToSwitchTo = m_ActiveScene;
    }

    std::shared_ptr<Scene> SceneManager::GetActiveScene()
    {
        MUSE_PROFILE_FUNCTION();

        ASSERT_ENGINE(m_ActiveScene != nullptr, "No scene active!")

        return m_ActiveScene;
    }

    void SceneManager::Clear()
    {
        MUSE_PROFILE_FUNCTION();

        if (m_ActiveScene != nullptr)
        {
            m_ActiveScene->Clear();
        }
    }

    void SceneManager::SwitchSceneImmediate(std::shared_ptr<Scene> a_NextScene)
    {
        MUSE_PROFILE_FUNCTION();

        if (m_ActiveScene != nullptr)
        {
            const std::string oldScenePath = GAME_SCENE_PATH + m_ActiveScene->GetName() + ".txt";
            ResourceManager::UnloadResource<Scene>(oldScenePath);
        }

        m_ActiveScene = a_NextScene;
    }
}
