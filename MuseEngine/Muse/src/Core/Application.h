#pragma once

#include "Window.h"
#include "LayerStack.h"

namespace Muse
{
    class Layer;
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

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
    protected:
        virtual void OnStart() = 0;
        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnFixedUpdate() = 0;
        virtual void OnRender() = 0;

    private:
        bool running = true;
        std::unique_ptr<Window> window;
        SystemManager* systemManager;
        LayerStack layerStack;

        bool OnWindowClose(WindowCloseEvent& windowCloseEvent);
	};

	// To be defined in client.
	Application* CreateApplication();
}
