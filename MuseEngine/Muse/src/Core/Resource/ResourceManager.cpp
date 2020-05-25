#include "MusePCH.h"

#include "Core/Resource/ResourceManager.h"

#include <string>
#include <filesystem>
#include "Core/Instrumentor.h"

namespace Muse
{
    std::unordered_map<ullong, std::shared_ptr<Resource>> ResourceManager::m_Resources;

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

    void ResourceManager::Add(const std::string& a_Name, const std::shared_ptr<Resource>& a_Resource)
    {
        ullong id = Resource::CalculateResourceId(a_Name);

        ASSERT_ENGINE(a_Name.find('/') == std::string::npos, "Use Load(a_FilePath) instead of Create when trying to load a file.");
        ASSERT_ENGINE(m_Resources.find(id) == m_Resources.end(), "Resource with name " + a_Name + " already exists!");

        std::dynamic_pointer_cast<Resource>(a_Resource)->InitPath(a_Name);

        m_Resources.insert(std::make_pair(id, a_Resource));
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

    void ResourceManager::UpdateResourcePath(const std::string& a_OldPath, const std::string& a_NewPath)
    {
        const ullong oldId = Resource::CalculateResourceId(a_OldPath);
        const ullong newId = Resource::CalculateResourceId(a_NewPath);

        const std::shared_ptr<Resource> resource = m_Resources.at(oldId);

        m_Resources[newId] = resource;

        m_Resources.erase(oldId);
    }
}
