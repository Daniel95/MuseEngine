#pragma once

#include "Core/ECS/Job/Job.h"

class MoveForwardJob : public Muse::Job
{
    virtual void OnUpdate() override;
};

