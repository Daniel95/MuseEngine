#pragma once

#include "Core/ECS/Job/Job.h"

class SpawnerJob : public Muse::Job
{
public:
    virtual void OnUpdate() override;

private:
    const float m_SpawnTime = 0.1f;
    float m_Timer;
    float m_SpawnMinX = -5;
    float m_SpawnMaxX = 5;
    float m_SpawnY = 7;

};

