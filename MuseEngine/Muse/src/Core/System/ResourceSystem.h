#pragma once

#include "Core/System/Manager/ISystem.h"
#include "Core/System/Resource/Resource.h"
#include "Core/Utilities/Defines.h"
#include "Core/Renderer/Shader.h"
#include "Core/System/Scene/Scene.h"

#include <unordered_map>
#include "Core/Renderer/Texture.h"

namespace Muse
{
    class SystemManager;

    class ResourceSystem : public ISystem, public SystemRegistry<ResourceSystem>
    {
        RTTR_ENABLE(ISystem);

    public:
        ResourceSystem(SystemManager& a_SystemManager);
        ~ResourceSystem() = default;

        template<typename T, typename... Args>
        std::shared_ptr<T> Load(const std::string & a_ResourcePath, Args&& ... a_Args);
        template<typename T>
        std::vector<std::shared_ptr<T>> GetResourcesOfType() const;
        template<typename T>
        void UnloadResource(const std::string & a_ResourcePath);
        std::vector<std::shared_ptr<Resource>> GetAllResources() const;

        void Initialize() override;
        void Terminate() override;

    protected:
        template<typename T>
        std::shared_ptr<T> CreateResource(const std::string& a_ResourcePath);
        template<>
        std::shared_ptr<Shader> CreateResource<Shader>(const std::string& a_ResourcePath);
        template<>
        std::shared_ptr<Scene> CreateResource<Scene>(const std::string& a_ResourcePath);
        template<>
        std::shared_ptr<Texture> CreateResource<Texture>(const std::string& a_ResourcePath);

        template<typename T>
        std::shared_ptr<T> GetLoadedResource(ullong a_Id);

        /// A map of resources
        std::unordered_map<ullong, std::shared_ptr<Resource>> m_Resources;

        /// A map of reference counters
        std::unordered_map<ullong, uint> m_RefCounters;

    private:
        bool m_InspectLoadedScenes = true;
    };

    template<typename T>
    std::vector<std::shared_ptr<T>> ResourceSystem::GetResourcesOfType() const
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
    std::shared_ptr<T> ResourceSystem::Load(const std::string & a_ResourcePath, Args&&... a_Args)
    {
        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Resource");

        ullong id = T::CalculateResourceId(a_ResourcePath);

        std::shared_ptr<T> resource = GetLoadedResource<T>(id);
        if (resource == nullptr)
        {
            resource = CreateResource<T>(a_ResourcePath);

            std::dynamic_pointer_cast<Resource>(resource)->SetPath(a_ResourcePath);

            m_Resources.insert(std::make_pair(id, resource));
            m_RefCounters.insert(std::make_pair(id, 1));
        }

        return resource;
    }

    template <typename T>
    std::shared_ptr<T> ResourceSystem::GetLoadedResource(ullong a_Id)
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
    void ResourceSystem::UnloadResource(const std::string & a_ResourcePath)
    {
        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Component");

        ullong id = T::CalculateResourceId(a_ResourcePath);

        ASSERT_ENGINE(m_Resources.find(id) != m_Resources.end(), "Resource does not exists!");

        m_Resources.erase(id);
        m_RefCounters.erase(id);
    }

    template <typename T>
    std::shared_ptr<T> ResourceSystem::CreateResource(const std::string& a_ResourcePath)
    {
        static_assert(std::is_abstract<T>(), "Cannot create this abstract resource.");
        std::shared_ptr<T> resource = std::make_shared<T>(a_ResourcePath);

        return resource;
    }

    template <>
    inline std::shared_ptr<Shader> ResourceSystem::CreateResource<Shader>(const std::string& a_ResourcePath)
    {
        return Shader::Create(a_ResourcePath);
    }

    template <>
    inline std::shared_ptr<Scene> ResourceSystem::CreateResource<Scene>(const std::string& a_ResourcePath)
    {
        return std::make_shared<Scene>();
    }

    template <>
    inline std::shared_ptr<Texture> ResourceSystem::CreateResource<Texture>(const std::string& a_ResourcePath)
    {
        return Texture::Create(a_ResourcePath);
    }
}
