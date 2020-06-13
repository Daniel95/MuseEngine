#pragma once
#include "Core/ECS/Component/ComponentHelper.h"

#include "Core/Physics/Collision2D.h"

#include "Core/ECS/Job/JobManager.h"
#include "Core/ECS/Component/Collider2DComponent.h"
#include "Core/ECS/Component/TransformComponent.h"
namespace Muse
{
	class Job
	{
	public:
		Job() = default;
		virtual ~Job() = default;
		
		void Update();
        virtual void OnUpdate() = 0;
		
        template <typename T1, typename T2>
        void Run(const std::function<void(int, T1&, T2&)>& a_Func);

        template <typename T1, typename T2>
        void RunCollision(const std::function<void(int, T1&, T2&)>& a_Func);

	};

	inline void Job::Update()
	{
		OnUpdate();
	}

    template <typename T1, typename T2>
    void Job::Run(const std::function<void(int, T1&, T2&)>& a_Func)
    {
        std::vector<int> entitiesWithComponents = ComponentHelper::GetEntitiesWith<T1, T2>();

        std::unordered_map<int, T1>& components1 = ComponentManager<T1>::GetComponents();
        std::unordered_map<int, T2>& components2 = ComponentManager<T2>::GetComponents();

        for (auto entity : entitiesWithComponents)
        {
            a_Func(entity, components1[entity], components2[entity]);
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
    void Job::RunCollision(const std::function<void(int, T1&, T2&)>& a_Func)
    {
        std::vector<int> entitiesWithComponents = ComponentHelper::GetEntitiesWith<T1, T2, Collider2DComponent, TransformComponent>();

        std::vector<std::pair<int, int>> hits;
        Collision2D::GetEntityHits(entitiesWithComponents, hits);

        std::unordered_map<int, T1>& components1 = ComponentManager<T1>::GetComponents();
        std::unordered_map<int, T2>& components2 = ComponentManager<T2>::GetComponents();

        for (auto hit : hits)
        {
            a_Func(hit.first, components1[hit.first], components2[hit.first]);
            a_Func(hit.second, components1[hit.second], components2[hit.first]);
        }
    }
}
