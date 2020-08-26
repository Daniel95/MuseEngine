#pragma once

#include <string>
#include <memory>

#include <rttr/registration>

namespace Muse 
{
    class Application;
    class GameObject;
    class SceneOld;
    class SystemManager;

    class SceneManagerOld
    {
        RTTR_ENABLE();

    public:
        //static SceneManagerOld& GetInstance()
        //{
        //    static SceneManagerOld instance;
        //    return instance;
        //}

        SceneManagerOld();
        virtual ~SceneManagerOld();

        void OnUpdate(float a_DeltaTime);

        //static std::shared_ptr<SceneOld> NewScene();
        //static void LoadScene(const std::string& a_SceneName);

        static void SwitchScene(std::shared_ptr<SceneOld> a_NextScene);
        static void SwitchScene(const std::string& a_SceneName);
        static void ReloadScene();
        static std::shared_ptr<SceneOld> GetActiveScene();
        static void DestroyAllGameObjects();

    private:
        static std::shared_ptr<SceneOld> m_SceneToSwitchTo;
        static std::shared_ptr<SceneOld> m_ActiveScene;

        static void SwitchSceneImmediate(std::shared_ptr<SceneOld> a_NextScene);

    };
}