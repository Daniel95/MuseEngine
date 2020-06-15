#pragma once

#include "Core/ECS/Job/Job.h"

class ObstacleCollisionJob : public Muse::Job
{
public:
    virtual void OnUpdate() override;

};
