#pragma once
#include "Core/System/Manager/ISystem.h"

namespace Muse
{
    class SoundSystem : public ISystem, public SystemRegistry<SoundSystem>
    {
        RTTR_ENABLE(ISystem);

    public:
        SoundSystem(SystemManager& a_SystemManager);
        ~SoundSystem();

        void Initialize();
        void Update(float a_DeltaTime);
        void Terminate();

    };
}
