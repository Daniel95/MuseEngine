#pragma once

#include "Core/ECS/Job/Job.h"

class ObstacleSpawnerJob : public Muse::Job
{
public:
    virtual void OnUpdate() override;

private:
    const float m_SpawnTime = 0.005f;
    float m_Timer;
    float m_SpawnMinX = -10;
    float m_SpawnMaxX = 10;
    float m_SpawnY = 10;

};

