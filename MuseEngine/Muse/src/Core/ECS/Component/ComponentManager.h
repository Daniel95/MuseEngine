#pragma once
#include "CORE/Utilities/Defines.h"
#include "Core/ECS/Entity/Entity.h"

#include <unordered_map>
#include <memory>
#include <variant>

namespace Muse
{
	template<class T>
	class ComponentManager
	{
	public:
		static void Add(Entity a_Entity, T a_Component);
		static void Remove(Entity a_Entity);
		static const std::unordered_map<int, T>& GetComponents() { return s_Components; }
		static T& Get(Entity a_Entity);
        static void Set(Entity a_Entity, T& a_Component);
		static bool Exist(Entity a_Entity) { return s_Components.find(a_Entity) != s_Components.end(); }
		
		static void OnEntityDestroy(int a_Entity);

	private:
        static std::unordered_map<int, T> s_Components;
        static ullong s_Id;

	};

	template<class T>
	std::unordered_map<int, T> ComponentManager<T>::s_Components;
	
	template <class T>
	void ComponentManager<T>::Add(Entity a_Entity, T a_Component)
	{
		ASSERT_ENGINE(!Exist(), "Component for this entity already exists!");
		
		//Entity::s_DestroyEvent;
		Entity::s_DestroyEvent.Subscribe([](int a_Entity) { ComponentManager<T>::OnEntityDestroy(a_Entity); });

		//LOG_INFO(Entity::s_DestroyEvent.GetSubscriptionCount());

		std::pair<int, T> pair{ a_Entity, a_Component };
		s_Components.insert(pair);
	}

	template <class T>
	void ComponentManager<T>::Remove(Entity a_Entity)
	{
		ASSERT_ENGINE(Exist(), "Component for this entity does not exist!");

		s_Components.erase(a_Entity);
	}

	template <class T>
	T& ComponentManager<T>::Get(Entity a_Entity)
	{
		ASSERT_ENGINE(Exist(), "Component for this entity does not exist!");

		return s_Components[a_Entity];
	}

	template <class T>
	void ComponentManager<T>::Set(Entity a_Entity, T& a_Component)
	{
		ASSERT_ENGINE(Exist(), "Component for this entity does not exist!");
		
		s_Components[a_Entity] = a_Component;
	}

	template<class T>
	inline void ComponentManager<T>::OnEntityDestroy(int a_Entity)
	{
		if (Exist(a_Entity))
		{
			LOG_INFO(s_Components.size());
            s_Components.erase(a_Entity);
			LOG_INFO(s_Components.size());

			//Remove(a_Entity);
		}
	}
}
