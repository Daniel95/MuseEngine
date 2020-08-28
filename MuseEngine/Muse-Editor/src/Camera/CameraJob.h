#pragma once

#include "Core/ECS/Job/Job.h"

namespace Muse
{
    class CameraJob : public Job
    {
        virtual void OnUpdate();
    };
}

