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
		int GetId() const { return m_Id; }

        static Entity Create();
        static Entity Create(const std::string& a_Name);
		static void Destroy(int a_Entity);
		
	private:
		 int m_Id = 0;
		
	};
}