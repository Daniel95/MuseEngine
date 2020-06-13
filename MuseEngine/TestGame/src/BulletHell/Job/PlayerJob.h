#pragma once

#include "Core/ECS/Job/Job.h"

class PlayerJob : public Muse::Job
{
public:
    virtual void OnUpdate() override;
};
