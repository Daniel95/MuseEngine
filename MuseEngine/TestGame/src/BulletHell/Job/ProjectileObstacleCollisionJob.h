#pragma once

#include "Core/ECS/Job/Job.h"

class ProjectileObstacleCollisionJob : public Muse::Job
{
public:
    virtual void OnUpdate() override;

};

