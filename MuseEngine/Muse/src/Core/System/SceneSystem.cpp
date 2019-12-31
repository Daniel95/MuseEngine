#include "MusePCH.h"

#include "Core/System/SceneSystem.h"

#include "Core/Application.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/BoxCollider2D.h"
#include "Core/System/ResourceSystem.h"
#include "Core/System/Manager/ISystem.h"
#include "Core/System/Manager/SystemManager.h"
#include "Core/System/Scene/Scene.h"
#include "Core/Utilities/Log.h"
#include "Core/Window.h"

namespace Muse 
{
    SceneSystem::SceneSystem(SystemManager& a_SystemManager, Application& a_Application)
        : ISystem(a_SystemManager)
    {
        m_Application = &a_Application;

        Application::Get().m_UpdateEvent.Subscribe(this, std::bind(&SceneSystem::OnUpdate, this, std::placeholders::_1));
    }

    SceneSystem::~SceneSystem()
    {
        Application::Get().m_UpdateEvent.Unsubscribe(this);

        if (m_ActiveScene != nullptr)
        {
            delete m_ActiveScene;
        }
    }

    void SceneSystem::Initialize()
    {
    }

    void SceneSystem::OnUpdate(float a_DeltaTime)
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

    void SceneSystem::Terminate()
    {
    }

    Scene& SceneSystem::NewScene()
    {
        if (m_ActiveScene != nullptr)
        {
            std::string oldScenePath = GAME_SCENE_PATH + m_ActiveScene->GetName() + ".txt";
            m_SystemManager.GetSystem<ResourceSystem>().UnloadResource<Scene>(oldScenePath);
        }

        std::string newScenePath = GAME_SCENE_PATH + "NewScene.txt";
        m_ActiveScene = m_SystemManager.GetSystem<ResourceSystem>().LoadResource<Scene>(newScenePath);

        return *m_ActiveScene;
    }

    void SceneSystem::LoadScene(const std::string& a_SceneName)
    {
        m_SceneNameToLoad = a_SceneName;
    }

    void SceneSystem::ReloadScene()
    {
        _ASSERT(m_ActiveScene != nullptr);

        m_SceneNameToLoad = m_ActiveScene->GetName();
    }

    Scene* SceneSystem::GetActiveScene()
    {
        return m_ActiveScene;
    }

    void SceneSystem::LoadSceneImmediate(const std::string& a_SceneName)
    {
        if (m_ActiveScene != nullptr)
        {
            const std::string oldScenePath = GAME_SCENE_PATH + m_ActiveScene->GetName() + ".txt";
            m_SystemManager.GetSystem<ResourceSystem>().UnloadResource<Scene>(oldScenePath);
        }

        const std::string newScenePath = GAME_SCENE_PATH + a_SceneName + ".txt";
        m_ActiveScene = m_SystemManager.GetSystem<ResourceSystem>().LoadResource<Scene>(newScenePath);
        m_ActiveScene->Load(GAME_SCENE_PATH, a_SceneName);
    }
}
