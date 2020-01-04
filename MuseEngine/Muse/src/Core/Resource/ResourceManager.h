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

namespace Muse
{
    class SystemManager;

    class ResourceManager
    {
        RTTR_ENABLE();

    public:
        template<typename T, typename... Args>
        static std::shared_ptr<T> Create(const std::string& a_Name, Args&& ... a_Args);
        template<typename T, typename... Args>
        static std::shared_ptr<T> Load(const std::string & a_ResourcePath, Args&& ... a_Args);
        template<typename T>
        static std::shared_ptr<T> Get(const std::string& a_ResourcePath);
        template<typename T>
        static std::vector<std::shared_ptr<T>> GetResourcesOfType();
        template<typename T>
        static void UnloadResource(const std::string & a_ResourcePath);
        static std::vector<std::shared_ptr<Resource>> GetAllResources();

        static void CreateSaveLocation();

    protected:
        template<typename T>
        static std::shared_ptr<T> CreateResource(const std::string& a_ResourcePath);
        template<>
        static std::shared_ptr<Shader> CreateResource<Shader>(const std::string& a_ResourcePath);
        template<>
        static std::shared_ptr<Scene> CreateResource<Scene>(const std::string& a_ResourcePath);
        template<>
        static std::shared_ptr<Texture> CreateResource<Texture>(const std::string& a_ResourcePath);

        template<typename T>
        static std::shared_ptr<T> GetLoadedResource(ullong a_Id);

        /// A map of resources
        static std::unordered_map<ullong, std::shared_ptr<Resource>> m_Resources;

        /// A map of reference counters
        static std::unordered_map<ullong, uint> m_RefCounters;

    };

    template<typename T>
    std::vector<std::shared_ptr<T>> ResourceManager::GetResourcesOfType()
    {
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
    std::shared_ptr<T> ResourceManager::Create(const std::string& a_Name, Args&&... a_Args)
    {
        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Resource");

        ullong id = T::CalculateResourceId(a_ResourcePath);

        ASSERT_ENGINE(a_ResourcePath.find("/") == std::string::npos, "Use Load(a_FilePath) instead of Create when trying to load a file.");

        std::shared_ptr<T> resource = GetLoadedResource<T>(id);
        ASSERT_ENGINE(resource == nullptr, "Resource is already loaded!");

        resource = CreateResource<T>(a_ResourcePath);

        std::dynamic_pointer_cast<Resource>(resource)->SetPathAndName(a_ResourcePath);

        m_Resources.insert(std::make_pair(id, resource));
        m_RefCounters.insert(std::make_pair(id, 1));

        return resource;
    }

    template<typename T, typename... Args>
    std::shared_ptr<T> ResourceManager::Load(const std::string & a_ResourcePath, Args&&... a_Args)
    {
        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Resource");

        ullong id = T::CalculateResourceId(a_ResourcePath);

        //ASSERT_ENGINE(std::filesystem::exists(a_ResourcePath), "Resource path doesn't points to a file.");

        std::shared_ptr<T> resource = GetLoadedResource<T>(id);
        ASSERT_ENGINE(resource == nullptr, "Resource is already loaded!");

        resource = CreateResource<T>(a_ResourcePath);

        std::dynamic_pointer_cast<Resource>(resource)->SetPathAndName(a_ResourcePath);

        m_Resources.insert(std::make_pair(id, resource));
        m_RefCounters.insert(std::make_pair(id, 1));

        return resource;
    }

    template <typename T>
    std::shared_ptr<T> ResourceManager::Get(const std::string& a_ResourcePath)
    {
        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Resource");

        ullong id = T::CalculateResourceId(a_ResourcePath);

        std::shared_ptr<T> resource = GetLoadedResource<T>(id);
        ASSERT_ENGINE(resource != nullptr, "Resource is not yet loaded!");

        return resource;
    }

    template <typename T>
    std::shared_ptr<T> ResourceManager::GetLoadedResource(ullong a_Id)
    {
        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Resource");

        auto it = m_Resources.find(a_Id);

        // Check cache
        if (it != m_Resources.end())
        {
            ++m_RefCounters[a_Id];
            return std::dynamic_pointer_cast<T>(it->second);
        }

        return nullptr;
    }

    template<typename T>
    void ResourceManager::UnloadResource(const std::string & a_ResourcePath)
    {
        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Component");

        ullong id = T::CalculateResourceId(a_ResourcePath);

        ASSERT_ENGINE(m_Resources.find(id) != m_Resources.end(), "Resource does not exists!");

        m_Resources.erase(id);
        m_RefCounters.erase(id);
    }

    template <typename T>
    std::shared_ptr<T> ResourceManager::CreateResource(const std::string& a_ResourcePath)
    {
        static_assert(std::is_abstract<T>(), "Cannot create this abstract resource.");
        std::shared_ptr<T> resource = std::make_shared<T>(a_ResourcePath);

        return resource;
    }

    template <>
    inline std::shared_ptr<Shader> ResourceManager::CreateResource<Shader>(const std::string& a_ResourcePath)
    {
        return Shader::Load(a_ResourcePath);
    }

    template <>
    inline std::shared_ptr<Scene> ResourceManager::CreateResource<Scene>(const std::string& a_ResourcePath)
    {
        return std::make_shared<Scene>();
    }

    template <>
    inline std::shared_ptr<Texture> ResourceManager::CreateResource<Texture>(const std::string& a_ResourcePath)
    {
        return Texture::Load(a_ResourcePath);
    }
}
