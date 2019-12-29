#pragma once

#include "Layer/LayerStack.h"
#include <memory>

namespace Muse
{
    class Layer;
    class GameWindow;
    class SceneSystem;
    class SystemManager;
    class ImGuiLayer;
    class Window;

	class Application
	{
	public:
        static Application& Get() { return *s_Instance; }

		Application();
		virtual ~Application();

        Window& GetWindow() const { return *m_Window; }
        SystemManager& GetSystemManager() const { return *m_SystemManager; }

        void Start();
        void Update();
        void FixedUpdate();
        void Render();

        void PushLayer(Layer* a_Layer);
        void PushOverlay(Layer* a_Layer);

    protected:
        virtual void OnStart() = 0;
        virtual void OnUpdate(float a_DeltaTime) = 0;
        virtual void OnFixedUpdate() = 0;
        virtual void OnRender() = 0;
        virtual void OnImGUIRender() = 0;

        virtual void OnWindowCloseEvent();
        virtual void OnWindowResizeEvent(int a_Width, int a_Height);
        virtual void OnKeyPressedEvent(int a_KeyCode, int a_RepeatCount);
        virtual void OnKeyReleasedEvent(int a_KeyCode);
        virtual void OnMouseButtonPressedEvent(int a_Button);
        virtual void OnMouseButtonReleasedEvent(int a_Button);
        virtual void OnMouseScrolledEvent(float a_XOffset, float a_YOffset);
        virtual void OnMouseMovedEvent(float a_X, float a_Y);

    private:
        static Application* s_Instance;
        bool m_Running = true;
        std::shared_ptr<Window> m_Window;

        ImGuiLayer* m_ImGuiLayer;
        SystemManager* m_SystemManager;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;
	};

	// To be defined in client.
	Application* CreateApplication();
}
