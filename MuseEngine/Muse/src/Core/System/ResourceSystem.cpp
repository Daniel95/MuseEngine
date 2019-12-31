#include "MusePCH.h"

#include "Core/System/Manager/SystemManager.h"
#include "Core/System/ResourceSystem.h"
#include "Core/Utilities/Log.h"

#include <string>
#include <iostream>
//#include <filesystem>

namespace Muse
{
    ResourceSystem::ResourceSystem(SystemManager& a_SystemManager)
        : ISystem(a_SystemManager)
    {
    }

    void ResourceSystem::Initialize()
    {
        char* pValue;
        size_t len;
        _dupenv_s(&pValue, &len, "APPDATA");

        std::string savePath = std::string(pValue) + std::string("\\Muse");

        if (!std::experimental::filesystem::exists(savePath))
        {
            std::experimental::filesystem::create_directory(savePath);
        }
    }

    std::vector<const Resource*> ResourceSystem::GetAllResources() const
    {
        std::vector<const Resource*> resources;

        for (std::pair<ullong, Resource*> resource : m_Resources)
        {
            resources.push_back(resource.second);
        }

        return resources;
    }

	void ResourceSystem::Terminate()
    {
    }
}
