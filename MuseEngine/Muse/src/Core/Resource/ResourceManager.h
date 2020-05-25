#pragma once

#include "Core/System/Manager/ISystem.h"
#include "Core/Utilities/Defines.h"
#include "Core/Scene/Scene.h"
#include "Core/Resource/Resource.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Texture.h"

#include <unordered_map>
#include <filesystem>
#include <fstream>
#include "Core/Scene/SceneManager.h"
#include "Core/Instrumentor.h"

namespace Muse
{
    class SystemManager;

    class ResourceManager
    {
        RTTR_ENABLE();

    public:
        static void Add(const std::string& a_Name, const std::shared_ptr<Resource>& a_Resource);
        template<typename T, typename... Args>
        static std::shared_ptr<T> Load(const std::string & a_Path);
        template<typename T>
        static std::shared_ptr<T> Get(const std::string& a_Path);
        template<typename T>
        static std::vector<std::shared_ptr<T>> GetResourcesOfType();
        template<typename T>
        static void UnloadResource(const std::string & a_Path);
        static std::vector<std::shared_ptr<Resource>> GetAllResources();
        static void UpdateResourcePath(const std::string& a_OldPath, const std::string& a_NewPath);

        static void CreateSaveLocation();

    protected:
        template<typename T>
        static std::shared_ptr<T> GetLoadedResource(ullong a_Id);

        /// A map of resources
        static std::unordered_map<ullong, std::shared_ptr<Resource>> m_Resources;
    };

    template<typename T>
    std::vector<std::shared_ptr<T>> ResourceManager::GetResourcesOfType()
    {
        MUSE_PROFILE_FUNCTION();

        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Component");

        std::vector<std::shared_ptr<T>> resources;

        for (auto const& x : m_Resources)
        {
            T* resourceOfType = dynamic_cast<T*>(x.second);

            if (resourceOfType != nullptr)
            {
                resources.push_back(resourceOfType);
            }
        }

        return resources;
    }

    template<typename T, typename... Args>
    std::shared_ptr<T> ResourceManager::Load(const std::string & a_Path)
    {
        MUSE_PROFILE_FUNCTION();

        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Resource");

        ullong id = T::CalculateResourceId(a_Path);

        ASSERT_ENGINE(std::filesystem::exists(a_Path), "Resource path doesn't points to a file.");
        ASSERT_ENGINE(GetLoadedResource<T>(id) == nullptr, "Resource is already loaded!");

        std::shared_ptr<T> resource = T::Load(a_Path);

        std::dynamic_pointer_cast<Resource>(resource)->InitPath(a_Path);

        m_Resources.insert(std::make_pair(id, resource));

        return resource;
    }

    template <typename T>
    std::shared_ptr<T> ResourceManager::Get(const std::string& a_Path)
    {
        MUSE_PROFILE_FUNCTION();

        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Resource");

        ullong id = T::CalculateResourceId(a_Path);

        std::shared_ptr<T> resource = GetLoadedResource<T>(id);
        ASSERT_ENGINE(resource != nullptr, "Resource is not yet loaded!");

        return resource;
    }

    template <typename T>
    std::shared_ptr<T> ResourceManager::GetLoadedResource(ullong a_Id)
    {
        MUSE_PROFILE_FUNCTION();

        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Resource");

        auto it = m_Resources.find(a_Id);

        // Check cache
        if (it != m_Resources.end())
        {
            return std::dynamic_pointer_cast<T>(it->second);
        }

        return nullptr;
    }

    template<typename T>
    void ResourceManager::UnloadResource(const std::string & a_Path)
    {
        MUSE_PROFILE_FUNCTION();

        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Component");

        ullong id = T::CalculateResourceId(a_Path);

        ASSERT_ENGINE(m_Resources.find(id) != m_Resources.end(), "Resource does not exists!");
        ASSERT_ENGINE(m_Resources[id].use_count() >= 1, "Resource " + a_Path + " that you are trying to delete is still in use!");

        m_Resources.erase(id);
    }
}
