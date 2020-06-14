#pragma once

#include "Core/Application.h"
#include "Core/Utilities/Defines.h"
#include "Core/Event/Event.h"

#include <vector>
#include <memory>

namespace Muse
{
    class DestroyEvent : public Event<int> {};

	class Entity
	{
	public:
		static DestroyEvent s_DestroyEvent;

		Entity(int a_Id);
		~Entity() = default;
		operator int() const { return m_Id; }

		static Entity Create();

		//template<typename T>
		//void AddComponent(T a_Component) const;
        //template<typename T>
        //void RemoveComponent() const;
		static void Destroy(int a_Entity);
		
		int GetId() const { return m_Id; }
		
	private:
		 int m_Id = 0;
		
	};

	/*
	template<typename T>
	inline void Entity::AddComponent(T a_Component) const
	{
	}

	template<typename T>
	inline void Entity::RemoveComponent() const
	{
	}
	*/

}