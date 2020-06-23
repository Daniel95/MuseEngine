#include "BackgroundParticleSpawner.h"

#include <glm/glm.hpp>

#include "BulletHell/Component/Components.h"
#include "BulletHell.h"
#include "Core/Utilities/Utilities.h"
#include "Core/Application.h"

void BackgroundParticleSpawner::OnUpdate()
{
    m_Timer += Muse::Application::Get().GetDeltaTime();

    if (m_Timer > m_SpawnTime)
    {
        float spawnX = Muse::Random(m_SpawnMinX, m_SpawnMaxX);
        glm::vec2 spawnPosition = glm::vec2(spawnX, m_SpawnY);

        m_Timer -= m_SpawnTime;

        BulletHell::CreateBackgroundParticle1(spawnPosition);
    }
}
