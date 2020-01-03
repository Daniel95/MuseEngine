#pragma once

#include <string>

namespace Muse 
{
    class Application;
    class GameObject;
    class Scene;
    class SystemManager;

    class SceneSystem
    {
        RTTR_ENABLE();

    public:
        /*
        static SceneSystem& GetInstance()
        {
            static SceneSystem instance;
            return instance;
        }
        */

        SceneSystem();
        virtual ~SceneSystem();

        void OnUpdate(float a_DeltaTime);

        static std::shared_ptr<Scene> NewScene();
        static void LoadScene(const std::string& a_SceneName);
        static void ReloadScene();
        static std::shared_ptr<Scene> GetActiveScene() { return m_ActiveScene; }

    private:
        static std::string m_SceneNameToLoad;
        static std::shared_ptr<Scene> m_ActiveScene;
        static bool m_InspectLoadedScenes;

        static void LoadSceneImmediate(const std::string& a_SceneName);

    };
}