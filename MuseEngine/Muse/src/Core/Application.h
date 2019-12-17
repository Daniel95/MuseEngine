#pragma once

#include "Window.h"
#include "LayerStack.h"
#include "Event.h"

namespace Muse
{
    class Layer;
    class WindowCloseEvent;
    class GameWindow;
    class SceneSystem;
    class SystemManager;
    class EventTest;

	class Application
	{
	public:
        inline static Application& Get() { return *s_Instance; }
        inline Window& GetWindow() { return *window; }

		Application();
		virtual ~Application();
        SystemManager* GetSystemManager() const;

        void Start();
        void Update();
        void FixedUpdate();
        void Render();

        void OnEvent(EventOld& event);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        static Event s_EventTest;

    protected:
        virtual void OnStart() = 0;
        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnFixedUpdate() = 0;
        virtual void OnRender() = 0;
        void Test();

    private:
        static Application* s_Instance;
        bool running = true;
        std::unique_ptr<Window> window;
        SystemManager* systemManager;
        LayerStack layerStack;

        bool OnWindowClose(WindowCloseEvent& windowCloseEvent);
	};

	// To be defined in client.
	Application* CreateApplication();
}
