#pragma once
#include "Core/System/Manager/SystemID.h"

#include <rttr/registration>

namespace Muse
{
    class SystemManager;

    class ISystem
    {
        RTTR_ENABLE();

    public:
        ISystem(SystemManager& a_SystemManager);
        virtual ~ISystem();
        ISystem(const ISystem&) = delete;

        virtual void Initialize() = 0;
        virtual void Terminate() = 0;

    protected:
        SystemManager& m_SystemManager;

    };
}
