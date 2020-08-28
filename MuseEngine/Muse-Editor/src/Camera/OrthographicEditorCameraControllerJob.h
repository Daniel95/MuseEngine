#pragma once

#include "Core/ECS/Job/Job.h"

namespace Muse
{
    class OrthographicEditorCameraControllerJob : public Job
    {
        virtual void OnUpdate();
    };
}
