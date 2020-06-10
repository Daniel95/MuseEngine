#pragma once

#include "Core/ECS/Job/Job.h"

namespace Muse
{
    class Render2DJob : public Job
    {
    public:
        virtual void OnUpdate() override;
    };
}