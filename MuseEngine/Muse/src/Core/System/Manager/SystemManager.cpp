#include "MusePCH.h"

#include "Core/System/Manager/SystemManager.h"

#include "Core/System/Manager/ISystem.h"
#include "Core/Application.h"

namespace Muse
{
    SystemManager::~SystemManager()
    {
        MUSE_PROFILE_FUNCTION();

        for (auto itr = m_Systems.begin(); itr != m_Systems.end(); itr++)
        {
            ISystem* system = itr->second;
            system->Terminate();
            delete system;
        }
    }

    std::vector<ISystem*> SystemManager::GetAllSystems()
    {
        MUSE_PROFILE_FUNCTION();

        std::vector<ISystem*> systemList = std::vector<ISystem*>();

        for (auto itr = m_Systems.begin(); itr != m_Systems.end(); ++itr)
        {
            systemList.push_back(itr->second);
        }

        return systemList;
    }

    void SystemManager::InitializeSystems()
    {
        MUSE_PROFILE_FUNCTION();

        for (auto itr = m_Systems.begin(); itr != m_Systems.end(); itr++)
        {
            itr->second->Initialize();
        }
    }
}
