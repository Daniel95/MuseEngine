#pragma once

#include "Core/ECS/Job/Job.h"

class PlayerObstacleCollisionJob : public Muse::Job
{
public:
    virtual void OnUpdate() override;

};
