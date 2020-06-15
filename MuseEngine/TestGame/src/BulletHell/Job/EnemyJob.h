#pragma once

#include "Core/ECS/Job/Job.h"

class EnemyJob : public Muse::Job
{
    virtual void OnUpdate() override;
};

