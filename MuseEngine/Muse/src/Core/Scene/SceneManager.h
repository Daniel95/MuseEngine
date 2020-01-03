#pragma once

#include <string>
#include <memory>

#include <rttr/registration>

namespace Muse 
{
    class Application;
    class GameObject;
    class Scene;
    class SystemManager;

    class SceneManager
    {
        RTTR_ENABLE();

    public:
        /*
        static SceneManager& GetInstance()
        {
            static SceneManager instance;
            return instance;
        }
        */

        SceneManager();
        virtual ~SceneManager();

        void OnUpdate(float a_DeltaTime);

        static std::shared_ptr<Scene> NewScene();
        static void LoadScene(const std::string& a_SceneName);
        static void ReloadScene();
        static std::shared_ptr<Scene> GetActiveScene();
        static void DestroyAllGameObjects();

    private:
        static std::string m_SceneNameToLoad;
        static std::shared_ptr<Scene> m_ActiveScene;
        static bool m_InspectLoadedScenes;

        static void LoadSceneImmediate(const std::string& a_SceneName);

    };
}