#include "MusePCH.h"

#include "Core/System/Manager/SystemManager.h"
#include "Core/System/SoundSystem.h"
#include "Core/Utilities/Log.h"

namespace Muse
{
    SoundSystem::SoundSystem(SystemManager& a_SystemManager)
        : ISystem(a_SystemManager)
    {

    }

    SoundSystem::~SoundSystem()
    {
        //LOG_ENGINE_WARN("Sound System terminated!");
    }

    void SoundSystem::Initialize()
    {

    }

    void SoundSystem::Update(float a_DeltaTime)
    {
        static float counter = 0;
        counter += a_DeltaTime;

        if (counter > 12.5f)
        {
            //LOG_ENGINE_INFO("Twelve and a half seconds have passed...");
            counter = 0;
        }
    }

    void SoundSystem::Terminate()
    {

    }
}
