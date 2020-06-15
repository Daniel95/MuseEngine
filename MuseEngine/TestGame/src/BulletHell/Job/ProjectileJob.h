#pragma once

#include "Core/ECS/Job/Job.h"

class ProjectileJob : public Muse::Job
{
    virtual void OnUpdate() override;
};

