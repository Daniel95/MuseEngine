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

        unsigned int GetWidth() const override { return width;  }
        unsigned int GetHeight() const override { return height; }
        void SetWidth(const unsigned int a_Width) override { width = a_Width; }
        void SetHeight(const unsigned int a_Height) override { height = a_Height; }

        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

    private:
        GLFWwindow* window;
        std::string title;
        unsigned int width, height;
        bool vSync;

        virtual void Init(const WindowProperties& properties);
        virtual void Shutdown();
    };
}
