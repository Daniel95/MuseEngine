#pragma once

#include "Window.h"
#include "Layer/LayerStack.h"
#include "Renderer/Buffer/IndexBuffer.h"

namespace Muse
{
    class Layer;
    class WindowCloseEvent;
    class GameWindow;
    class SceneSystem;
    class SystemManager;
    class ImGuiLayer;
    class Shader;
    class VertexBuffer;

	class Application
	{
	public:
        inline static Application& Get() { return *s_Instance; }

		Application();
		virtual ~Application();

        Window& GetWindow() const { return *m_Window; }
        SystemManager& GetSystemManager() const { return *m_SystemManager; }

        void Start();
        void Update();
        void FixedUpdate();
        void Render();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

    protected:
        virtual void OnStart() = 0;
        virtual void OnUpdate(float m_DeltaTime) = 0;
        virtual void OnFixedUpdate() = 0;
        virtual void OnRender() = 0;

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
        std::unique_ptr<Window> m_Window;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        ImGuiLayer* m_ImGuiLayer;
        SystemManager* m_SystemManager;
        LayerStack m_LayerStack;
        unsigned int m_VertexArray;
	};

	// To be defined in client.
	Application* CreateApplication();
}
