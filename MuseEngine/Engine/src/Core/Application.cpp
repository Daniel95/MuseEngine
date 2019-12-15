#include "OatPCH.h"

#include "Core/Application.h"

#include "Core/System/ResourceSystem.h"
#include "Core/System/SceneSystem.h"
#include "Core/System/SoundSystem.h"
#include "Core/System/Manager/SystemManager.h"
#include "Core/Utilities/Log.h"
#include "Core/Utilities/Defines.h"
#include "Core/System/Event/ApplicationEvent.h"

#include <GLFW/glfw3.h>

namespace Muse
{
#define BIND_EVENT_FN_TEMP(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application()
    {
        window = std::unique_ptr<Window>(Window::Create());
        window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

        systemManager = new SystemManager();

        systemManager->CreateSystem<ResourceSystem>();
        systemManager->CreateSystem<SoundSystem>();
        systemManager->CreateSystem<SceneSystem>(*this);
    }

    Application::~Application()
    {
        delete systemManager;
    }

    SystemManager* Application::GetSystemManager() const
    {
        return systemManager;
    }

	void Application::Start()
	{
        OnStart();

        while (running)
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
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        window->OnUpdate();

        OnRender();
	}

    void Application::OnEvent(Event& event)
    {
        LOG_ENGINE_TRACE("Event {0}", event.ToString());

        EventDispatcher eventDispatcher(event);

        eventDispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN_TEMP(OnWindowClose));
    }

    bool Application::OnWindowClose(WindowCloseEvent& windowCloseEvent)
    {
        running = false;
        return true;
    }
}
