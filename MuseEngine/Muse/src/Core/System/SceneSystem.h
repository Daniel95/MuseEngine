#pragma once
#include "Core/System/Manager/ISystem.h"

#include <string>

namespace Muse 
{
    class Application;
    class GameObject;
    class Scene;
    class SystemManager;

    class SceneSystem : public ISystem, public SystemRegistry<SceneSystem>
    {
        RTTR_ENABLE(ISystem);

    public:
        SceneSystem(SystemManager& a_SystemManager, Application& a_Application);
        ~SceneSystem();

        void Initialize() override;
        void Terminate() override;
        void OnUpdate(float a_DeltaTime);

        std::shared_ptr<Scene> NewScene();
        void LoadScene(const std::string& a_SceneName);
        void ReloadScene();
        std::shared_ptr<Scene> GetActiveScene() { return m_ActiveScene; }

    private:
        Application* m_Application;
        std::string m_SceneNameToLoad;

        std::shared_ptr<Scene> m_ActiveScene = nullptr;
        bool m_InspectLoadedScenes = false;

        void LoadSceneImmediate(const std::string& a_SceneName);

    };
}