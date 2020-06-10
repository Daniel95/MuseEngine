#pragma once

#include "Core/Application.h"
#include "Core/Utilities/Defines.h"

#include <vector>
#include <memory>

namespace Muse
{
	class Entity
	{
	public:
		Entity(int a_Id);
		~Entity() = default;
		operator int() const { return m_Id; }

		template<typename T>
		void AddComponent(T a_Component) const;
		
		int GetId() const { return m_Id; }
		
		static Entity Create();
		
	private:
		 int m_Id = 0;
		
	};

}