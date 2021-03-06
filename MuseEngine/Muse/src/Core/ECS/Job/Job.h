#pragma once
#include "Core/ECS/Entity/Entity.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/SceneManager.h"

#include "entt.hpp"

namespace Muse
{
	class Job
	{
	public:
		Job() = default;
		virtual ~Job() = default;
		
        void Update() { OnUpdate(); };
        virtual void OnUpdate() = 0;
		
        template <typename T1, typename T2>
        void Run(const std::function<void(Entity, T1&, T2&)>& a_Func);

        template <typename T1, typename T2, typename T3>
        void Run(const std::function<void(Entity, T1&, T2&, T3&)>& a_Func);

        void RunCollision(const std::function<void(Entity, Entity)>& a_Func);

        /*
        template <typename T1, typename T2>
        void RunCollision(const std::function<void(Entity, T1&, T2&)>& a_Func1, const std::function<void(Entity, T1&, T2&)>& a_Func2);

        template <typename T1, typename T2>
        void RunCollision(const std::function<void(Entity, T1&, Entity, T2&)>& a_Func);

        template <typename T1, typename T2>
        void RunCollision(const std::function<void(Entity, T1&, TransformComponent&, Entity, T2&, TransformComponent&)>& a_Func);
        */

        //template <typename T1, typename T2, typename T3, typename T4>
        //void RunCollision(const std::function<void(int, T1&, T2&, int, T3&, T4&)>& a_Func);
	};

    template <typename T1, typename T2>
    void Job::Run(const std::function<void(Entity, T1&, T2&)>& a_Func)
    {
        std::shared_ptr<Scene> scene = SceneManager::GetActiveScene();

        auto view = scene->GetRegistry().view<T1, T2>();

        for (auto entity : view)
        {
            a_Func({ entity }, view.get<T1>(entity), view.get<T2>(entity));
        }
    }

    template <typename T1, typename T2, typename T3>
    void Job::Run(const std::function<void(Entity, T1&, T2&, T3&)>& a_Func)
    {
        std::shared_ptr<Scene> scene = SceneManager::GetActiveScene();

        auto view = scene->GetRegistry().view<T1, T2, T3>();

        for (auto entity : view)
        {
            a_Func({ entity }, view.get<T1>(entity), view.get<T2>(entity), view.get<T3>(entity));
        }
    }

    /*
    /// <summary>
    /// Executes the lambda when a entity that has component T1 collides with another entity that has T2.
    /// Is executes for both entities that collide with each other.
    /// </summary>
    /// <typeparam name="T1">This Component</typeparam>
    /// <typeparam name="T2">Other Component</typeparam>
    /// <param name="a_Func"The Lambda></param>
    template <typename T1, typename T2>
    void Job::RunCollision(const std::function<void(Entity, T1&, Entity, T2&)>& a_Func)
    {
        const std::vector<std::pair<Entity, Entity>>& collidingEntities = Collision2D::GetCollidingEntities();

        collidingEntities








        std::shared_ptr<Scene> scene = SceneManager::GetActiveScene();

        auto view1 = scene->GetRegistry().view<T1, Collider2DComponent, TransformComponent>();
        auto view2 = scene->GetRegistry().view<T2, Collider2DComponent, TransformComponent>();

        if (view1.size() == 0 || view2.size() == 0) { return; }

        const entt::entity* view1Entities = view1.data();
        const entt::entity* view2Entities = view2.data();

        std::vector<std::pair<Entity, Entity>> hits;
        Collision2D::GetEntityHits(view1, view2, hits);

        for (auto hit : hits)
        {
            a_Func(hit.first, ComponentManager<T1>::Get(hit.first), hit.second, ComponentManager<T2>::Get(hit.second));
        }
    }

    /// <summary>
    /// Executes the lambda when a entity that has component T1 collides with another entity that has T2.
    /// Is executes for both entities that collide with each other.
    /// </summary>
    /// <typeparam name="T1">This Component</typeparam>
    /// <typeparam name="T2">Other Component</typeparam>
    /// <param name="a_Func"The Lambda></param>
    template <typename T1, typename T2>
    void Job::RunCollision(const std::function<void(Entity, T1&, TransformComponent& , Entity, T2&, TransformComponent&)>& a_Func)
    {
        const std::vector<int> entityGroup1 = ComponentHelper::GetEntitiesWith<T1, Collider2DComponent, TransformComponent>();
        const std::vector<int> entityGroup2 = ComponentHelper::GetEntitiesWith<T2, Collider2DComponent, TransformComponent>();

        if (entityGroup1.empty() || entityGroup2.empty()) { return; }

        std::vector<std::pair<int, int>> hits;
        Collision2D::GetEntityHits(entityGroup1, entityGroup2, hits);

        for (auto hit : hits)
        {
            a_Func(
                hit.first, 
                ComponentManager<T1>::Get(hit.first),
                ComponentManager<TransformComponent>::Get(hit.first), 
                hit.second, 
                ComponentManager<T2>::Get(hit.second), 
                ComponentManager<TransformComponent>::Get(hit.second)
            );
        }
    }

    /// <summary>
    /// Executes the lambda when a entity that has component T1 collides with another entity that has T2.
    /// Is executes for both entities that collide with each other.
    /// </summary>
    /// <typeparam name="T1">This Component</typeparam>
    /// <typeparam name="T2">Other Component</typeparam>
    /// <param name="a_Func"The Lambda></param>
    template <typename T1, typename T2>
    void Job::RunCollision(const std::function<void(Entity, T1&, T2&)>& a_Func1, const std::function<void(Entity, T1&, T2&)>& a_Func2)
    {
        const std::vector<int> entityGroup1 = ComponentHelper::GetEntitiesWith<T1, Collider2DComponent, TransformComponent>();
        const std::vector<int> entityGroup2 = ComponentHelper::GetEntitiesWith<T2, Collider2DComponent, TransformComponent>();

        if (entityGroup1.empty() || entityGroup2.empty()) { return; }

        std::vector<std::pair<int, int>> hits;
        Collision2D::GetEntityHits(entityGroup1, entityGroup2, hits);

        for (auto hit : hits)
        {
            a_Func1(hit.first, ComponentManager<T1>::Get(hit.first), ComponentManager<T2>::Get(hit.second));
            a_Func2(hit.second, ComponentManager<T1>::Get(hit.first), ComponentManager<T2>::Get(hit.second));
        }
    }
    */
}
