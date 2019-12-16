#pragma once

#include "Core/System/Manager/ISystem.h"
#include "Core/System/Resource/Resource.h"
#include "Core/Utilities/Defines.h"

#include <unordered_map>
#include <cassert>

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
        T* LoadResource(const std::string & a_ResourcePath, Args&& ... args);
        template<typename T>
        std::vector<T*> GetResourcesOfType() const;
        template<typename T>
        void UnloadResource(const std::string & a_ResourcePath);
        std::vector<const Resource*> GetAllResources() const;

        void Initialize() override;
        void Update(float a_DeltaTime) override;
		void LateUpdate();
        void Terminate() override;

    protected:
        template<typename T>
        T* GetLoadedResource(ullong id);

        /// A map of resources
        std::unordered_map<ullong, Resource*> m_Resources;

        /// A map of reference counters
        std::unordered_map<ullong, uint> m_RefCounters;

    private:
        bool m_InspectLoadedScenes = true;
    };

    template<typename T>
    inline std::vector<T*> ResourceSystem::GetResourcesOfType() const
    {
        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Component");

        std::vector<T*> resources;

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
    T* ResourceSystem::LoadResource(const std::string & a_ResourcePath, Args&&... args)
    {
        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Resource");

        ullong id = T::CalculateResourceID(a_ResourcePath);

        T* resource = GetLoadedResource<T>(id);
        if (resource == nullptr)
        {
            resource = new T(id, a_ResourcePath, args...);
            m_Resources.insert(std::make_pair(id, resource));
            m_RefCounters.insert(std::make_pair(id, 1));
        }

        return resource;
    }

    template <typename T>
    T* ResourceSystem::GetLoadedResource(ullong id)
    {
        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Resource");

        auto it = m_Resources.find(id);

        // Check cache
        if (it != m_Resources.end())
        {
            ++m_RefCounters[id];
            return dynamic_cast<T*>(it->second);
        }

        return nullptr;
    }

    template<typename T>
    void ResourceSystem::UnloadResource(const std::string & a_ResourcePath)
    {
        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Component");

        ullong id = T::CalculateResourceID(a_ResourcePath);

        _ASSERT(m_Resources.find(id) != m_Resources.end());

        T* resource = GetLoadedResource<T>(id);

        m_Resources.erase(id);
        m_RefCounters.erase(id);

        delete resource;
    }
}
