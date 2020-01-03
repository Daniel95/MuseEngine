#include "MusePCH.h"

#include "Core/System/Manager/SystemManager.h"
#include "Core/System/ResourceSystem.h"

#include <string>
#include <filesystem>

namespace Muse
{
    std::unordered_map<ullong, std::shared_ptr<Resource>> ResourceSystem::m_Resources;
    std::unordered_map<ullong, uint> ResourceSystem::m_RefCounters;

    void ResourceSystem::CreateSaveLocation()
    {
        char* pValue;
        size_t len;
        _dupenv_s(&pValue, &len, "APPDATA");

        std::string savePath = std::string(pValue) + std::string("\\Muse");

        if (!std::filesystem::exists(savePath))
        {
            std::filesystem::create_directory(savePath);
        }
    }

    std::vector<std::shared_ptr<Resource>> ResourceSystem::GetAllResources()
    {
        std::vector<std::shared_ptr<Resource>> resources;

        for (std::pair<ullong, std::shared_ptr<Resource>> resource : m_Resources)
        {
            resources.push_back(resource.second);
        }

        return resources;
    }
}
