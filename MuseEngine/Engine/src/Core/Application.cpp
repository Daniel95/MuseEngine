#include "OatPCH.h"

#include "Core/Application.h"

#include "Core/System/ResourceSystem.h"
#include "Core/System/SceneSystem.h"
#include "Core/System/SoundSystem.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/BoxCollider2D.h"
#include "Core/System/Manager/SystemManager.h"
#include "Core/System/Scene/Scene.h"
#include "Core/Utilities/Log.h"
#include "Core/System/EventSystem.h"
#include "Core/System/Event/ApplicationEvent.h"

namespace Muse
{
    Application::Application()
    {
        systemManager = new SystemManager();

        systemManager->CreateSystem<ResourceSystem>();
        systemManager->CreateSystem<SoundSystem>();
        systemManager->CreateSystem<SceneSystem>(*this);
    }

    Application::~Application()
    {
    }

    SystemManager* Application::GetSystemManager()
    {
        return systemManager;
    }

	void Application::Start()
	{
        OnStart();

        //EventDispatcher dispatcher(AppUpdateEvent);
        //dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));

        while (true)
        {
            Update();
            FixedUpdate();
            Render();
        }
	}

	void Application::Update()
	{
        systemManager->UpdateSystems(0);
        OnUpdate(0.016f);
	}

    void Application::FixedUpdate()
    {
        OnFixedUpdate();
    }

	void Application::Render()
	{
        OnRender();
	}
}