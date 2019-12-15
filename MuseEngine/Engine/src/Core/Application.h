#pragma once

#include <vector>
#include <chrono>
#include "Window.h"

namespace Muse
{
    class WindowCloseEvent;
    class GameWindow;
    class SceneSystem;
    class SystemManager;

	class Application
	{
	public:
		Application();
		virtual ~Application();
        SystemManager* GetSystemManager() const;

        void Start();
        void Update();
        void FixedUpdate();
        void Render();

        void OnEvent(Event& event);
    protected:
        virtual void OnStart() = 0;
        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnFixedUpdate() = 0;
        virtual void OnRender() = 0;

    private:
        bool running = true;
        std::unique_ptr<Window> window;
        SystemManager* systemManager;

        bool OnWindowClose(WindowCloseEvent& windowCloseEvent);
	};

	// To be defined in client.
	Application* CreateApplication();
}