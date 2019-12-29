#include "PlayerComponent.h"
#include "Core/Input/Input.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Gameplay/Component/TransformComponent.h"

void PlayerComponent::Update(float a_DeltaTime)
{
    if (Muse::Input::IsKeyPressed(MUSE_KEY_A))
    {
        GetTransform()->Move(glm::vec2(-m_MoveSpeed * a_DeltaTime, 0.0f));
    }
    else if (Muse::Input::IsKeyPressed(MUSE_KEY_D))
    {
        GetTransform()->Move(glm::vec2(m_MoveSpeed * a_DeltaTime, 0.0f));
    }

    if (Muse::Input::IsKeyPressed(MUSE_KEY_S))
    {
        GetTransform()->Move(glm::vec2(0.0f, -m_MoveSpeed * a_DeltaTime));
    }
    else if (Muse::Input::IsKeyPressed(MUSE_KEY_W))
    {
        GetTransform()->Move(glm::vec2(0.0f, m_MoveSpeed * a_DeltaTime));
    }
}
