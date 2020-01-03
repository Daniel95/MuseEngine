#include "PlayerComponent.h"

#include "Muse.h"

#include "glm/glm.hpp"

void PlayerComponent::OnUpdate(float a_DeltaTime)
{
    if (Muse::Input::IsKeyPressed(MUSE_KEY_LEFT))
    {
        GetTransform()->Move(glm::vec2(-m_MoveSpeed * a_DeltaTime, 0.0f));
    }
    else if (Muse::Input::IsKeyPressed(MUSE_KEY_RIGHT))
    {
        GetTransform()->Move(glm::vec2(m_MoveSpeed * a_DeltaTime, 0.0f));
    }

    if (Muse::Input::IsKeyPressed(MUSE_KEY_DOWN))
    {
        GetTransform()->Move(glm::vec2(0.0f, -m_MoveSpeed * a_DeltaTime));
    }
    else if (Muse::Input::IsKeyPressed(MUSE_KEY_UP))
    {
        GetTransform()->Move(glm::vec2(0.0f, m_MoveSpeed * a_DeltaTime));
    }
}
