#pragma once

#include <unordered_map>

namespace Muse
{
    class EntityDebugger
    {
    public:
        static void SetComponentEntities(const std::string& a_ComponentName, const std::vector<int>& a_Entities);
        static const std::unordered_map<int, std::vector<std::string>>& GetEntityData() { return s_EntityData; }
        static void ClearEntityData() { return s_EntityData.clear(); }
        static void AddEntityName(int a_Entity, const std::string& a_Name) { s_EntityNames[a_Entity] = a_Name; };
        static void RemoveEntityName(int a_Entity) { s_EntityNames.erase(a_Entity); };
        static bool HasEntityName(int a_Entity) { return s_EntityNames.count(a_Entity) != 0; };
        static const std::string& GetEntityName(int a_Entity) { return s_EntityNames.at(a_Entity); };

    private:
        static std::unordered_map<int, std::vector<std::string>> s_EntityData;
        static std::unordered_map<int, std::string> s_EntityNames;

    };
}
