#pragma once

#include "Core/Window.h"

struct GLFWwindow;

namespace Muse
{
    class GraphicsContext;

    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProperties& a_Properties);
        virtual ~WindowsWindow();

        virtual void OnUpdate() override;

        virtual unsigned int GetWidth() const override { return m_Width;  }
        virtual unsigned int GetHeight() const override { return m_Height; }
        virtual void SetWidth(const unsigned int a_Width) override { m_Width = a_Width; }
        virtual void SetHeight(const unsigned int a_Height) override { m_Height = a_Height; }

        virtual void SetVSync(bool a_Enabled) override;
        virtual bool IsVSync() const override;
        virtual void* GetNativeWindow() const override { return m_Window; }

    private:
        GLFWwindow* m_Window;
        GraphicsContext* m_Context;
        std::string m_Title;
        unsigned int m_Width, m_Height;
        bool vSync;

        virtual void Init(const WindowProperties& a_Properties);
        virtual void Shutdown();
    };
}
