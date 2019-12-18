#include "MusePCH.h"

#include "Core/Application.h"

#include "Core/System/ResourceSystem.h"
#include "Core/System/SceneSystem.h"
#include "Core/System/SoundSystem.h"
#include "Core/System/Manager/SystemManager.h"
#include "Core/Utilities/Log.h"
#include "Core/Utilities/Defines.h"
#include "Core/Event/ApplicationEvent.h"
#include "Core/Layer.h"

#include <glad/glad.h>

namespace Muse
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        ASSERT(!s_Instance, "A instance of Application already exists!");
        s_Instance = this;

        window = std::unique_ptr<Window>(Window::Create());
        window->WindowCloseEvent.Subscribe(this, std::bind(&Application::OnWindowClose, this));
        window->WindowResizeEvent.Subscribe(this, std::bind(&Application::OnWindowResize, this, std::placeholders::_1, std::placeholders::_2));

        window->WindowCloseEvent.Dispatch();
        window->WindowResizeEvent.Dispatch(100, 100);

        systemManager = new SystemManager();

        systemManager->CreateSystem<ResourceSystem>();
        systemManager->CreateSystem<SoundSystem>();
        systemManager->CreateSystem<SceneSystem>(*this);
    }

    Application::~Application()
    {
        window->WindowCloseEvent.Unsubscribe(this);
        window->WindowResizeEvent.Unsubscribe(this);

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
            for (Layer* layer : layerStack)
            {
                layer->OnUpdate(0.16f);
            }

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
        window->OnUpdate();

        OnRender();
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
	}

    /*
    void Application::OnEvent(EventOld& event)
    {
        LOG_ENGINE_TRACE("EventOld {0}", event.ToString());

        EventDispatcher eventDispatcher(event);

        eventDispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN_TEMP(OnWindowClose));

        for (auto it = layerStack.end(); it != layerStack.begin();)
        {
            (*--it)->OnEvent(event);
            if(event.Handled)
            {
                break;
            }
        }
    }
    */

    void Application::PushLayer(Layer* layer)
    {
        layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        layerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::Test()
    {
    }

    void Application::OnWindowClose()
    {
        LOG_ENGINE_INFO("OnWindowClose");
        running = false;
    }

    void Application::OnWindowResize(int width, int height)
    {
        LOG_ENGINE_INFO("OnWindowResize: {0}, {0}", width, height);
    }
}
