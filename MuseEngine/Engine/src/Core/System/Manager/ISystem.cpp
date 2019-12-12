#include "OatPCH.h"

#include "Core/System/Manager/ISystem.h"
#include "Core/System/Manager/SystemManager.h"

#include "Core/Utilities/Log.h"

namespace Muse
{
    ISystem::ISystem(SystemManager& a_SystemManager)
        : m_SystemManager(a_SystemManager)
    {
    }

    ISystem::~ISystem()
    {

    }
}
