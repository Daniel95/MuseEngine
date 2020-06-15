#pragma once
#include "Core/Utilities/Defines.h"
#include "Core/ECS/Entity/Entity.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

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
		ASSERT_ENGINE(!Exist(a_Entity), "Component for this entity already exists!");
		
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
		ASSERT_ENGINE(Exist(a_Entity), "Component for this entity does not exist!");

		s_Components.erase(a_Entity);
	}

	template <class T>
	T& ComponentManager<T>::Get(Entity a_Entity)
	{
		ASSERT_ENGINE(Exist(a_Entity), "Component for this entity does not exist!");

		return s_Components[a_Entity];
	}

	template <class T>
	void ComponentManager<T>::Set(Entity a_Entity, T& a_Component)
	{
		ASSERT_ENGINE(Exist(a_Entity), "Component for this entity does not exist!");
		
		s_Components[a_Entity] = a_Component;
	}

	template<class T>
	inline void ComponentManager<T>::OnEntityDestroy(int a_Entity)
	{
		if (Exist(a_Entity))
		{
			s_EntitiesToRemove.push_back(a_Entity);
		}
	}

	template<class T>
	inline void ComponentManager<T>::Register(const std::string& a_RegisterName)
	{
        s_Name = a_RegisterName;
        s_Initiated = true;

        Entity::s_DestroyEvent.Subscribe([](int a_Entity)
        {
            ComponentManager<T>::OnEntityDestroy(a_Entity);
        });

        Application::Get().m_UpdateEvent.Subscribe([](float a_DeltaTime)
        {
            ComponentManager<T>::Update(a_DeltaTime);
        });

		Application::Get().m_LateUpdateEvent.Subscribe([](float a_DeltaTime)
        {
            ComponentManager<T>::LateUpdate(a_DeltaTime);
        });
	}

	template<class T>
	inline void ComponentManager<T>::Update(float a_DeltaTime)
	{

	}

	template<class T>
	inline void ComponentManager<T>::LateUpdate(float a_DeltaTime)
	{
		for (auto entity : s_EntitiesToRemove)
		{
			Remove(entity);
		}
		s_EntitiesToRemove.clear();
	}
}
