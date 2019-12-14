#include "OatPCH.h"

#include "Core/Application.h"

#include "Core/System/ResourceSystem.h"
#include "Core/System/SceneSystem.h"
#include "Core/System/SoundSystem.h"
#include "Core/System/Manager/SystemManager.h"
#include "Core/Utilities/Log.h"

namespace Muse
{
    Application::Application()
    {
        window = std::unique_ptr<Window>(Window::Create());




        systemManager = new SystemManager();

        systemManager->CreateSystem<ResourceSystem>();
        systemManager->CreateSystem<SoundSystem>();
        systemManager->CreateSystem<SceneSystem>(*this);
    }

    Application::~Application()
    {
        delete systemManager;
    }

    SystemManager* Application::GetSystemManager()
    {
        return systemManager;
    }

	void Application::Start()
	{
        OnStart();

        //EventDispatcher dispatcher(AppUpdateEvent);
        //dispatcher.Dispatch<WindowCloseEvent>(MUSE_BIND_EVENT_FN(Application::OnWindowClose));

        while (true)
        {

            Update();
            FixedUpdate();
            Render();
        }
	}

	void Application::Update()
	{
        window->OnUpdate();


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