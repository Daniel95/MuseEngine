#include "PlayerComponent.h"

#include "Muse.h"

#include "glm/glm.hpp"

void PlayerComponent::OnUpdate(float a_DeltaTime)
{
    if (Muse::Input::GetKeyDown(MUSE_KEY_LEFT))
    {
        GetTransform()->Translate(glm::vec2(-m_MoveSpeed * a_DeltaTime, 0.0f));
    }
    else if (Muse::Input::GetKeyDown(MUSE_KEY_RIGHT))
    {
        GetTransform()->Translate(glm::vec2(m_MoveSpeed * a_DeltaTime, 0.0f));
    }

    if (Muse::Input::GetKeyDown(MUSE_KEY_DOWN))
    {
        GetTransform()->Translate(glm::vec2(0.0f, -m_MoveSpeed * a_DeltaTime));
    }
    else if (Muse::Input::GetKeyDown(MUSE_KEY_UP))
    {
        GetTransform()->Translate(glm::vec2(0.0f, m_MoveSpeed * a_DeltaTime));
    }
}
