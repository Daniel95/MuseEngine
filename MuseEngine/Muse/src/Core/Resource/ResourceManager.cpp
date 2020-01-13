#include "MusePCH.h"

#include "Core/Resource/ResourceManager.h"

#include <string>
#include <filesystem>
#include "Core/Instrumentor.h"

namespace Muse
{
    std::unordered_map<ullong, std::shared_ptr<Resource>> ResourceManager::m_Resources;
    std::unordered_map<ullong, uint> ResourceManager::m_RefCounters;

    void ResourceManager::CreateSaveLocation()
    {
        MUSE_PROFILE_FUNCTION();

        char* pValue;
        size_t len;
        _dupenv_s(&pValue, &len, "APPDATA");

        std::string savePath = std::string(pValue) + std::string("\\Muse");

        if (!std::filesystem::exists(savePath))
        {
            std::filesystem::create_directory(savePath);
        }
    }

    std::vector<std::shared_ptr<Resource>> ResourceManager::GetAllResources()
    {
        MUSE_PROFILE_FUNCTION();

        std::vector<std::shared_ptr<Resource>> resources;

        for (std::pair<ullong, std::shared_ptr<Resource>> resource : m_Resources)
        {
            resources.push_back(resource.second);
        }

        return resources;
    }

    void ResourceManager::UpdateResourcePath(const std::string& a_OldResourcePath, const std::string& a_NewResourcePath)
    {
        ullong oldId = Resource::CalculateResourceId(a_OldResourcePath);
        ullong newId = Resource::CalculateResourceId(a_NewResourcePath);

        std::shared_ptr<Resource> resource = m_Resources.at(oldId);
        int refCount = m_RefCounters.at(oldId);

        m_Resources[newId] = resource;
        m_RefCounters[newId] = refCount;

        m_Resources.erase(oldId);
        m_RefCounters.erase(oldId);
    }
}
