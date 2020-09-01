#include "MusePCH.h"
#include "Job.h"

#include "Core/Physics/Collision2D.h"

namespace Muse
{
    void Job::RunCollision(const std::function<void(Entity, Entity)>& a_Func)
    {
        const std::vector<std::pair<Entity, Entity>>& collidingEntities = Collision2D::GetCollidingEntities();

        for (const auto& pair : collidingEntities)
        {
            a_Func(pair.first, pair.second);
        }
    }
}