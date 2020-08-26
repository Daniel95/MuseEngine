#include "MusePCH.h"

#include "Core/Scene/SceneManagerOld.h"
#include "Core/Application.h"
#include "Core/Resource//ResourceManager.h"
#include "Core/Scene/SceneOld.h"
#include "Core/Utilities/Log.h"
#include "Core/Utilities/Defines.h"
#include "Core/Window.h"

namespace Muse 
{
    std::shared_ptr<SceneOld> SceneManagerOld::m_ActiveScene = nullptr;
    std::shared_ptr<SceneOld> SceneManagerOld::m_SceneToSwitchTo = nullptr;

    SceneManagerOld::SceneManagerOld()
    {
        MUSE_PROFILE_FUNCTION();

        Application::Get().m_UpdateEvent.Subscribe(SUB_FN(SceneManagerOld::OnUpdate, std::placeholders::_1));
    }

    SceneManagerOld::~SceneManagerOld()
    {
        MUSE_PROFILE_FUNCTION();

        Application::Get().m_UpdateEvent.Unsubscribe(this);
    }

    void SceneManagerOld::OnUpdate(float a_DeltaTime)
    {
        MUSE_PROFILE_FUNCTION();

        if (m_SceneToSwitchTo != nullptr)
        {
            SwitchSceneImmediate(m_SceneToSwitchTo);
            m_SceneToSwitchTo = nullptr;
        }

        if (m_ActiveScene != nullptr)
        {
            m_ActiveScene->Update(a_DeltaTime);
        }
    }

    void SceneManagerOld::SwitchScene(std::shared_ptr<SceneOld> a_NextScene)
    {
        if (m_ActiveScene != nullptr)
        {
            ResourceManager::UnloadResource<SceneOld>(m_ActiveScene->GetPath());
        }

        m_SceneToSwitchTo = a_NextScene;
    }

    void SceneManagerOld::SwitchScene(const std::string& a_SceneName)
    {
        const std::shared_ptr<SceneOld> scene = ResourceManager::Get<SceneOld>(a_SceneName);

        if (m_ActiveScene != nullptr)
        {
            ResourceManager::UnloadResource<SceneOld>(m_ActiveScene->GetPath());
        }

        m_SceneToSwitchTo = scene;
    }

    void SceneManagerOld::ReloadScene()
    {
        MUSE_PROFILE_FUNCTION();

        ASSERT_ENGINE(m_ActiveScene != nullptr, "There is not active scene to reload!");

        m_SceneToSwitchTo = m_ActiveScene;
    }

    std::shared_ptr<SceneOld> SceneManagerOld::GetActiveScene()
    {
        MUSE_PROFILE_FUNCTION();

        return m_ActiveScene;
    }

    void SceneManagerOld::DestroyAllGameObjects()
    {
        MUSE_PROFILE_FUNCTION();

        if(m_ActiveScene != nullptr)
        {
            m_ActiveScene->DestroyAllGameObjects();
        }
    }

    void SceneManagerOld::SwitchSceneImmediate(std::shared_ptr<SceneOld> a_NextScene)
    {
        MUSE_PROFILE_FUNCTION();

        if (m_ActiveScene != nullptr)
        {
            const std::string oldScenePath = GAME_SCENE_PATH + m_ActiveScene->GetName() + ".txt";
            ResourceManager::UnloadResource<SceneOld>(oldScenePath);
        }

        m_ActiveScene = a_NextScene;
    }
}
