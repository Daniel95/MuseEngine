#pragma once
#include "ComponentManager.h"

namespace Muse
{
	template<typename T1, typename T2>
	class ComponentPair
	{
	public:
		const T1 component1;
		const T2 component2;
	};

	class ComponentHelper
	{
	public:
		template<typename T1, typename T2>
		static std::vector<int> GetEntitiesWith();
        template<typename T1, typename T2, typename T3>
        static std::vector<int> GetEntitiesWith();
        template<typename T1, typename T2, typename T3, typename T4>
        static std::vector<int> GetEntitiesWith();
	};

	template <typename T1, typename T2>
	std::vector<int> ComponentHelper::GetEntitiesWith()
	{
		const std::unordered_map<int, T1>& components1 = ComponentManager<T1>::GetComponents();
		const std::unordered_map<int, T2>& components2 = ComponentManager<T2>::GetComponents();

		std::vector<int> overlappingEntities;

		for (auto it = components1.begin(); it != components1.end(); ++it)
		{
			if (components2.find(it->first) != components2.end())
			{
				overlappingEntities.push_back(it->first);
			}
		}

		return overlappingEntities;
	}

    template <typename T1, typename T2, typename T3>
    std::vector<int> ComponentHelper::GetEntitiesWith()
    {
        const std::unordered_map<int, T1>& components1 = ComponentManager<T1>::GetComponents();
        const std::unordered_map<int, T2>& components2 = ComponentManager<T2>::GetComponents();
        const std::unordered_map<int, T3>& components3 = ComponentManager<T3>::GetComponents();

        std::vector<int> overlappingEntities;
		
        for (auto it = components1.begin(); it != components1.end(); ++it)
        {
            if (components2.find(it->first) != components2.end() &&
				components3.find(it->first) != components3.end())
            {
                overlappingEntities.push_back(it->first);
            }
        }

        return overlappingEntities;
    }

    template <typename T1, typename T2, typename T3, typename T4>
    std::vector<int> ComponentHelper::GetEntitiesWith()
    {
        const std::unordered_map<int, T1>& components1 = ComponentManager<T1>::GetComponents();
        const std::unordered_map<int, T2>& components2 = ComponentManager<T2>::GetComponents();
        const std::unordered_map<int, T3>& components3 = ComponentManager<T3>::GetComponents();
        const std::unordered_map<int, T4>& components4 = ComponentManager<T4>::GetComponents();

        std::vector<int> overlappingEntities;

        for (auto it = components1.begin(); it != components1.end(); ++it)
        {
            if (components2.find(it->first) != components2.end() &&
                components3.find(it->first) != components3.end() &&
                components4.find(it->first) != components4.end())
            {
                overlappingEntities.push_back(it->first);
            }
        }

        return overlappingEntities;
    }
}
