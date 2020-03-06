#pragma once
#include "Core/Gameplay/Component/Component.h"

class PlayerComponent : public Muse::Component
{
public:
    virtual void OnUpdate(float a_DeltaTime) override;

private:
    float m_MoveSpeed = 0.3f;
    float m_RotateSpeed = 0.3f;
    float m_ScaleSpeed = 0.3f;

};
