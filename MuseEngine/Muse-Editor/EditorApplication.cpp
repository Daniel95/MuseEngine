#include "Muse.h"
#include "EntryPoint.h"

#include "EditorLayer.h"

namespace Muse
{
    class EditorApplication : public Application
    {
    public:
        EditorApplication() : Application("Muse Editor")
        {
            PushLayer(new EditorLayer());
        }

        ~EditorApplication() = default;

    protected:
        virtual void OnStart() override {};
        virtual void OnUpdate(float a_DeltaTime) override {}
        virtual void OnFixedUpdate() override {}
        virtual void OnRender() override {};
        virtual void OnImGuiRender() override {};
    };

    Application* CreateApplication()
    {
        return new EditorApplication();
    }

}