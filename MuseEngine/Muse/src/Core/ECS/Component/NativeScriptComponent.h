#pragma once
#include <Core/ECS/Component/ScriptableEntity.h>

namespace Muse
{
    struct NativeScriptComponent
    {
        ScriptableEntity* Instance = nullptr;

        std::function<void()> InstantiateFunction;
        std::function<void()> DestroyInstanceFunction;

        std::function<void(ScriptableEntity* a_Instance)> OnCreateFunction;
        std::function<void(ScriptableEntity* a_Instance)> OnDestroyFunction;
        std::function<void(ScriptableEntity* a_Instance, float a_DeltaTime)> OnUpdateFunction;

        template<typename T>
        void Bind()
        {
            InstantiateFunction = [&]() { Instance = new T(); };
            DestroyInstanceFunction = [&]() { delete (T*)Instance; Instance = nullptr; };

            OnCreateFunction = [](ScriptableEntity* a_Instance) { ((T*)a_Instance)->OnCreate(); };
            OnDestroyFunction = [](ScriptableEntity* a_Instance) { ((T*)a_Instance)->OnDestroy(); };
            OnUpdateFunction = [](ScriptableEntity* a_Instance, float a_DeltaTime) { ((T*)a_Instance)->OnUpdate(a_DeltaTime); };
        }
    };

}