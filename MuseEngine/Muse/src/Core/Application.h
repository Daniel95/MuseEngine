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
        inline static Application& Get() { return *s_Instance; }

		Application();
		virtual ~Application();

        inline Window& GetWindow() { return *window; }
        SystemManager* GetSystemManager() const;

        void Start();
        void Update();
        void FixedUpdate();
        void Render();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

    protected:
        virtual void OnStart() = 0;
        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnFixedUpdate() = 0;
        virtual void OnRender() = 0;

    private:
        static Application* s_Instance;
        bool running = true;
        std::unique_ptr<Window> window;
        SystemManager* systemManager;
        LayerStack layerStack;

        void OnWindowClose();
        void OnWindowResize(int a_Width, int a_Height);
        void KeyPressedEvent(int a_KeyCode, int a_RepeatCount);
        void OnKeyReleasedEvent(int a_KeyCode);
        void OnMouseButtonPressedEvent(int a_Button);
        void OnMouseButtonReleasedEvent(int a_Button);
        void OnMouseScrolledEvent(float a_XOffset, float a_YOffset);
        void OnMouseMovedEvent(float a_X, float a_Y);

	};

	// To be defined in client.
	Application* CreateApplication();
}
