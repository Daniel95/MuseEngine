#include "OatPCH.h"

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

        std::string RaymanPCSavePath = std::string(pValue) + std::string("\\RayMan");

        if (!std::experimental::filesystem::exists(RaymanPCSavePath))
        {
            std::experimental::filesystem::create_directory(RaymanPCSavePath);
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

    void ResourceSystem::Update(float a_DeltaTime)
    {
        a_DeltaTime;
    }

	void ResourceSystem::LateUpdate()
	{

	}

	void ResourceSystem::Terminate()
    {
    }
}
