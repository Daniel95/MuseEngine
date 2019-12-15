#pragma once

#include "Core/Window.h"

struct GLFWwindow;

namespace Muse
{
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProperties& properties);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return data.Width;  }
        inline unsigned int GetHeight() const override { return data.Height; }

        inline void SetEventCallback(const EventCallbackFn& callback) override { data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

    private:
        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData data;
        GLFWwindow* window;

        virtual void Init(const WindowProperties& properties);
        virtual void Shutdown();
    };
}
