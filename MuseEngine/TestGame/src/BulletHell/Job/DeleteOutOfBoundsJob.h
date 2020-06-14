#pragma once

#include "Core/ECS/Job/Job.h"

class DeleteOutOfBoundsJob : public Muse::Job
{
public:
    virtual void OnUpdate() override;

};

