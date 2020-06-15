#pragma once

#include "Core/ECS/Job/Job.h"

class DestroyOutOfBoundsJob : public Muse::Job
{
public:
    virtual void OnUpdate() override;

};

