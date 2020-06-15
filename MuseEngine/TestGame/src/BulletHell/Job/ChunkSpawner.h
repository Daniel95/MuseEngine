#pragma once
#include "Core/ECS/Job/Job.h"

class ChunkSpawner : public Muse::Job
{
    virtual void OnUpdate() override;
};

