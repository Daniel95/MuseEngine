#include "MusePCH.h"
#include "Scene.h"
#include "Core\ECS\Component\TransformComponent.h"
#include "Core\ECS\Component\Collider2DComponent.h"
#include "Core/Instrumentor.h"
#include "Core/ECS/Entity/Entity.h"

#include "cereal\archives\json.hpp"

namespace Muse
{
    Scene::Scene()
    {
        entt::entity entity = m_Registry.create();

        m_Registry.emplace<TransformComponent>(entity);

        m_Registry.view<TransformComponent, Collider2DComponent>();

    }

    Entity Scene::CreateEntity()
    {
        Entity entity = { m_Registry.create() };
        entity.AddComponent<TransformComponent>();
        return entity;
    }

    Entity Scene::CreateEntity(TransformComponent& a_TransformComponent)
    {
        Entity entity = { m_Registry.create() };
        entity.AddComponent<TransformComponent>(a_TransformComponent);
        return entity;
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
