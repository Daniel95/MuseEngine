#pragma once
#include "Core/ECS/Job/Job.h"

class PlayerMovementJob : public Muse::Job
{
public:
    virtual void OnUpdate() override;
};
