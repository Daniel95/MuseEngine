#pragma once

#include <vector>
#include <chrono>
#include "Window.h"

namespace sf 
{
	class RenderWindow;
}

namespace Muse
{
    class GameWindow;
    class SceneSystem;
    class SystemManager;

	class Application
	{
	public:
		Application();
		virtual ~Application();
        SystemManager* GetSystemManager();

        void Start();
        void Update();
        void FixedUpdate();
        void Render();
    protected:
        virtual void OnStart() = 0;
        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnFixedUpdate() = 0;
        virtual void OnRender() = 0;

    private:
        SystemManager* systemManager;
        std::unique_ptr<Window> window;
        bool running = true;

	};

	// To be defined in client.
	Application* CreateApplication();
}