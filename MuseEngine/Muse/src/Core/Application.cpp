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
        window->KeyPressedEvent.Subscribe(this, std::bind(&Application::OnWindowResize, this, std::placeholders::_1, std::placeholders::_2));
        window->KeyReleasedEvent.Subscribe(this, std::bind(&Application::OnKeyReleasedEvent, this, std::placeholders::_1));
        window->MouseButtonPressedEvent.Subscribe(this, std::bind(&Application::OnMouseButtonPressedEvent, this, std::placeholders::_1));
        window->MouseButtonReleasedEvent.Subscribe(this, std::bind(&Application::OnMouseButtonReleasedEvent, this, std::placeholders::_1));
        window->MouseScrolledEvent.Subscribe(this, std::bind(&Application::OnMouseScrolledEvent, this, std::placeholders::_1, std::placeholders::_2));
        window->MouseMovedEvent.Subscribe(this, std::bind(&Application::OnMouseMovedEvent, this, std::placeholders::_1, std::placeholders::_2));

        systemManager = new SystemManager();

        systemManager->CreateSystem<ResourceSystem>();
        systemManager->CreateSystem<SoundSystem>();
        systemManager->CreateSystem<SceneSystem>(*this);
    }

    Application::~Application()
    {
        window->WindowCloseEvent.Unsubscribe(this);
        window->WindowResizeEvent.Unsubscribe(this);
        window->KeyPressedEvent.Unsubscribe(this);
        window->KeyReleasedEvent.Unsubscribe(this);
        window->MouseButtonPressedEvent.Unsubscribe(this);
        window->MouseButtonReleasedEvent.Unsubscribe(this);
        window->MouseScrolledEvent.Unsubscribe(this);
        window->MouseMovedEvent.Unsubscribe(this);

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

    void Application::OnWindowResize(int a_Width, int a_Height)
    {
        LOG_ENGINE_INFO("OnWindowResize: {0}, {1}", a_Width, a_Height);
    }

    void Application::KeyPressedEvent(int a_KeyCode, int a_RepeatCount)
    {
        LOG_ENGINE_INFO("KeyPressedEvent: {0}, {1}", a_KeyCode, a_RepeatCount);
    }

    void Application::OnKeyReleasedEvent(int a_KeyCode)
    {
        LOG_ENGINE_INFO("OnKeyReleasedEvent: {0}", a_KeyCode);
    }

    void Application::OnMouseButtonPressedEvent(int a_Button)
    {
        LOG_ENGINE_INFO("OnMouseButtonPressedEvent: {0}", a_Button);
    }

    void Application::OnMouseButtonReleasedEvent(int a_Button)
    {
        LOG_ENGINE_INFO("OnMouseButtonReleasedEvent: {0}", a_Button);
    }

    void Application::OnMouseScrolledEvent(float a_XOffset, float a_yOffset)
    {
        LOG_ENGINE_INFO("OnMouseScrolledEvent: {0}, {1}", a_XOffset, a_yOffset);
    }

    void Application::OnMouseMovedEvent(float a_X, float a_Y)
    {
        LOG_ENGINE_INFO("OnMouseMovedEvent: {0}, {1}", a_X, a_Y);
    }
}
