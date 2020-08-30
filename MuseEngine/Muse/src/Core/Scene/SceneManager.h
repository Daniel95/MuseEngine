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
    public:
        SceneManager();
        virtual ~SceneManager();

        void OnUpdate(float a_DeltaTime);

        static void SwitchScene(std::shared_ptr<Scene> a_NextScene);
        static void SwitchScene(const std::string& a_SceneName);
        static void ReloadScene();
        static std::shared_ptr<Scene> GetActiveScene();
        static void Clear();

    private:
        static std::shared_ptr<Scene> m_SceneToSwitchTo;
        static std::shared_ptr<Scene> m_ActiveScene;

        static void SwitchSceneImmediate(std::shared_ptr<Scene> a_NextScene);

    };
}