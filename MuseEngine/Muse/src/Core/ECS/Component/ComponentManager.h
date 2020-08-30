#pragma once
#include "Core/Utilities/Defines.h"
#include "Core/ECS/Entity/Entity.h"
#include "Core/ECS/Entity/EntityDebugger.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <cstring>

namespace Muse
{
	/*
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
        static void OnEntityDestroy(Entity a_Entity);
        static void OnEntityDestroyAll();
		static std::vector<int> GetEntities();
		static void Register(const std::string& a_RegisterName);
        static void Update(float a_DeltaTime);
        static void LateUpdate(float a_DeltaTime);

	private:
        static std::unordered_map<int, T> s_Components;
        static std::vector<int> s_EntitiesToRemove;
		static bool s_Initiated;
		static std::string s_Name;
		static const std::string s_UnregisteredName;

	};

	template<class T>
	std::unordered_map<int, T> ComponentManager<T>::s_Components;

    template<class T>
	std::vector<int> ComponentManager<T>::s_EntitiesToRemove;

    template<class T>
	bool ComponentManager<T>::s_Initiated = false;

    template<class T>
	const std::string ComponentManager<T>::s_UnregisteredName = "Unregistered";

    template<class T>
    std::string ComponentManager<T>::s_Name = "";

	template <class T>
	void ComponentManager<T>::Add(Entity a_Entity, T a_Component)
	{
		ASSERT_ENGINE(!Exist(a_Entity), "Component for this entity already exists: " + a_Entity.GetName());
		
		if (!s_Initiated)
		{
			Register(s_UnregisteredName);
		}

		std::pair<int, T> pair{ a_Entity, a_Component };
		s_Components.insert(pair);
	}

	template <class T>
	void ComponentManager<T>::Remove(Entity a_Entity)
	{
		ASSERT_ENGINE(Exist(a_Entity), "Component for this entity does not exist: " + a_Entity.GetName());

		s_Components.erase(a_Entity);
	}

	template <class T>
	T& ComponentManager<T>::Get(Entity a_Entity)
	{
        ASSERT_ENGINE(Exist(a_Entity), "Component for this entity does not exist: " + a_Entity.GetName());

		return s_Components[a_Entity];
	}

	template <class T>
	void ComponentManager<T>::Set(Entity a_Entity, T& a_Component)
	{
        ASSERT_ENGINE(Exist(a_Entity), "Component for this entity does not exist: " + a_Entity.GetName());
		
		s_Components[a_Entity] = a_Component;
	}

	template<class T>
	inline void ComponentManager<T>::OnEntityDestroy(Entity a_Entity)
	{
		if (Exist(a_Entity) && std::find(s_EntitiesToRemove.begin(), s_EntitiesToRemove.end(), a_Entity) == s_EntitiesToRemove.end())
		{
            s_EntitiesToRemove.push_back(a_Entity);
		}
	}

	template<class T>
	inline void ComponentManager<T>::OnEntityDestroyAll()
	{
        for (auto pair : s_Components)
        {
			if (std::find(s_EntitiesToRemove.begin(), s_EntitiesToRemove.end(), pair.first) == s_EntitiesToRemove.end())
			{
				s_EntitiesToRemove.push_back(pair.first);
			}
        }
	}

	template<class T>
	inline std::vector<int> ComponentManager<T>::GetEntities()
	{
		std::vector<int> entities;

		for (auto pair : s_Components)
		{
			entities.push_back(pair.first);
		}

		return entities;
	}

	template<class T>
	inline void ComponentManager<T>::Register(const std::string& a_RegisterName)
	{
        s_Name = a_RegisterName;
        s_Initiated = true;

		/*
        Entity::s_DestroyEvent.Subscribe([](Entity a_Entity)
        {
            ComponentManager<T>::OnEntityDestroy(a_Entity);
        });

        Entity::s_DestroyAllEvent.Subscribe([]()
        {
            ComponentManager<T>::OnEntityDestroyAll();
        });

#ifdef MUSE_DEBUG
        Application::Get().m_UpdateEvent.Subscribe([](float a_DeltaTime)
        {
            ComponentManager<T>::Update(a_DeltaTime);
        });
#endif

		
		
		Application::Get().m_LateUpdateEvent.Subscribe([](float a_DeltaTime)
        {
            ComponentManager<T>::LateUpdate(a_DeltaTime);
        });
	}
		*/

	/*
	template<class T>
	inline void ComponentManager<T>::Update(float a_DeltaTime)
	{
		EntityDebugger::SetComponentEntities(s_Name, GetEntities());
	}
	*/

	/*
	template<class T>
	inline void ComponentManager<T>::LateUpdate(float a_DeltaTime)
	{
		for (auto entity : s_EntitiesToRemove)
		{
			Remove(entity);
		}
		s_EntitiesToRemove.clear();
	}
	*/
}
