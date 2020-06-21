#pragma once

#include "Core/ECS/Job/Job.h"

#include "glm/glm.hpp"

class ObstacleCollisionJob : public Muse::Job
{
public:
    virtual void OnUpdate() override;

    static void SpawnBullets(glm::vec2 a_Position, float a_ProjectileSpeed = 10.0f);

};
