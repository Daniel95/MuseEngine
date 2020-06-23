#include "ObstacleSpawnerJob.h"

#include <glm/glm.hpp>

#include "Core/Application.h"
#include "Core/Utilities/Utilities.h"

#include "BulletHell/Component/Components.h"
#include "BulletHell.h"

void ObstacleSpawnerJob::OnUpdate()
{
    m_Timer += Muse::Application::Get().GetDeltaTime();

    if (m_Timer > m_SpawnTime)
    {
        float spawnX = Muse::Random(m_SpawnMinX, m_SpawnMaxX);
        glm::vec2 spawnPosition = glm::vec2(spawnX, m_SpawnY);

        m_Timer -= m_SpawnTime;

        if (Muse::Random() > 0.3f)
        {
            BulletHell::CreateObstacle(spawnPosition);
        }
        else
        {
            BulletHell::CreateEnemy(spawnPosition);
        }
    }
}
