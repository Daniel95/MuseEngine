#include "MusePCH.h"
#include "EntityDebugger.h"

#include "Core/Application.h"
#include "Core/Utilities/Defines.h"

#include <string>

namespace Muse
{
    std::unordered_map<int, std::vector<std::string>> EntityDebugger::s_EntityData;
    std::unordered_map<int, std::string> EntityDebugger::s_EntityNames;

    void EntityDebugger::SetComponentEntities(const std::string& a_ComponentName, const std::vector<int>& a_Entities)
    {
        bool entityFound = false;

        for (auto entity : a_Entities)
        {
            for (auto& pair : s_EntityData)
            {
                if (entity == pair.first)
                {
                    pair.second.push_back(a_ComponentName);

                    entityFound = true;
                    break;
                }
            }

            if (!entityFound)
            {
                s_EntityData[entity] = std::vector<std::string> { a_ComponentName };
            }
        }
    }
}