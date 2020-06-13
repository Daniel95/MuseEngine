#pragma once

#include "Core/ECS/Job/Job.h"

class HealthJob : public Muse::Job
{
public:
    virtual void OnUpdate() override;
};
