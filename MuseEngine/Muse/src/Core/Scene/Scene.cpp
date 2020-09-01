#include "MusePCH.h"
#include "Scene.h"

#include "cereal\archives\json.hpp"

#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/Collider2DComponent.h"
#include "Core/Instrumentor.h"
#include "Core/ECS/Entity/Entity.h"
#include "Core/ECS/Component/NativeScriptComponent.h"
#include "Core/ECS/Component/TagComponent.h"
#include "Core/ECS/Component/CameraComponent.h"

namespace Muse
{
    Entity Scene::CreateEntity(const std::string& a_Name)
    {
        Entity entity = { m_Registry.create() };
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.m_Tag = a_Name.empty() ? "Entity" : a_Name;
        return entity;
    }

    Entity Scene::CreateEntity(const std::string& a_Name, TransformComponent& a_TransformComponent)
    {
        Entity entity = { m_Registry.create() };
        entity.AddComponent<TransformComponent>(a_TransformComponent);
        auto& tag = entity.AddComponent<TagComponent>();
        tag.m_Tag = a_Name.empty() ? "Entity" : a_Name;
        return entity;
    }

    void Scene::DestroyEntity(Entity a_Entity)
    {

    }

    std::shared_ptr<Scene> Scene::Load(const std::string& a_FilePath)
    {
        std::filesystem::path path{ a_FilePath }; //creates TestingFolder object on C:
        std::filesystem::create_directories(path.parent_path()); //add directories based on the object path (without this line it will not work)
        std::ifstream fs(path);

        std::shared_ptr<Scene> scene = std::make_shared<Scene>();

        {
            cereal::JSONInputArchive iarchive(fs);
            iarchive(cereal::make_nvp(a_FilePath, *scene.get()));
        }
        fs.close();

        return scene;
    }

    void Scene::OnUpdate(float a_DeltaTime)
    {
        // Update script
        {
            m_Registry.view<NativeScriptComponent>().each([=](Entity a_Entity, NativeScriptComponent& a_NCS)
            {
                // TODO: Move to Scene::OnScenePlay
                if (a_NCS.Instance == nullptr)
                {
                    a_NCS.Instance = a_NCS.InstantiateScript();
                    a_NCS.Instance->m_Entity = Entity{ a_Entity };
                    a_NCS.Instance->OnCreate();
                }

                a_NCS.Instance->OnUpdate(a_DeltaTime);
            });
        }

        for (auto entity : m_EntitiesToRemove)
        {
            m_Registry.remove(entity.GetEntityHandle());
        }
        m_EntitiesToRemove.clear();
    }

    void Scene::OnViewportResize(uint32_t a_Width, uint32_t a_Height)
    {
        m_ViewportWidth = a_Width;
        m_ViewportHeight = a_Height;

        //Resize our non-FixedAspectRatio camera's

        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& camera = view.get<CameraComponent>(entity);
            if (!camera.GetFixedAspectRatio())
            {
                camera.SetViewportSize(a_Width, a_Height);
            }
        }

        const entt::entity* entities = view.data();

        auto test = view.data();
    }

    void Scene::Save(const std::string& a_FilePath)
    {
        MUSE_PROFILE_FUNCTION();

        m_Registry.clear();

        std::string txtExtension = ".txt";
        std::string savePath = a_FilePath;

        if (a_FilePath.find(txtExtension) == std::string::npos)
        {
            savePath += txtExtension;
        }

        UpdatePath(savePath);
        std::filesystem::path path{ savePath }; //creates TestingFolder object on C:
        std::filesystem::create_directories(path.parent_path()); //add directories based on the object path (without this line it will not work)
        std::ofstream ofs(path);

        {
            cereal::JSONOutputArchive oarchive(ofs);
            oarchive(cereal::make_nvp(savePath, *this));
        }
        ofs.close();
    }

    void Scene::Clear()
    {
        m_Registry.clear();
    }
}
