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
        void Update(float a_DeltaTime) override;
        void FixedUpdate(float a_TimeStep);
        void Terminate() override;

        Scene& NewScene();
        void LoadScene(const std::string& a_SceneName);
        void ReloadScene();
        Scene* GetActiveScene();
        
    private:
        void LoadSceneImmediate(const std::string& a_SceneName);

    private:
        Application* m_Application;
        std::string m_SceneNameToLoad;

        Scene* m_ActiveScene = nullptr;
        bool m_InspectLoadedScenes = false;


    };
}