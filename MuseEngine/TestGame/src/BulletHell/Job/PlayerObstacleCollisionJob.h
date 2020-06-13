#pragma once

#include "Core/ECS/Job/Job.h"

class PlayerObstacleCollisionJob : Muse::Job
{
public:
    virtual void OnUpdate() override;

};
