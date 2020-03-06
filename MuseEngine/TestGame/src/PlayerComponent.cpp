#include "PlayerComponent.h"

#include "Muse.h"

#include "glm/glm.hpp"

void PlayerComponent::OnUpdate(float a_DeltaTime)
{
    //Translate
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

    //Rotate
    if (Muse::Input::GetKeyDown(MUSE_KEY_Z))
    {
        GetTransform()->RotateLocal(glm::vec3(0.0f, 0.0f, -m_RotateSpeed * a_DeltaTime));
    }
    else if (Muse::Input::GetKeyDown(MUSE_KEY_X))
    {
        GetTransform()->RotateLocal(glm::vec3(0.0f, 0.0f, m_RotateSpeed * a_DeltaTime));
    }
	
    //Scale
    if (Muse::Input::GetKeyDown(MUSE_KEY_T))
    {
        GetTransform()->ScaleLocal(glm::vec2(-m_ScaleSpeed * a_DeltaTime, 0.0f));
    }
    else if (Muse::Input::GetKeyDown(MUSE_KEY_G))
    {
        GetTransform()->ScaleLocal(glm::vec2(m_ScaleSpeed * a_DeltaTime, 0.0f));
    }

    if (Muse::Input::GetKeyDown(MUSE_KEY_H))
    {
        GetTransform()->ScaleLocal(glm::vec2(0.0f, -m_ScaleSpeed * a_DeltaTime));
    }
    else if (Muse::Input::GetKeyDown(MUSE_KEY_F))
    {
        GetTransform()->ScaleLocal(glm::vec2(0.0f, m_ScaleSpeed * a_DeltaTime));
    }
}
