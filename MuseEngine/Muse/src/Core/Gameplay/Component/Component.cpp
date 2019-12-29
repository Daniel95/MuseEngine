#include "MusePCH.h"

#include "Core/Gameplay/Component/Component.h"
#include "Core/Gameplay/GameObject.h"
#include "TransformComponent.h"

namespace Muse 
{
    class BoxCollider2D;

    void Component::Init(GameObject* a_GameObject)
    {
        m_GameObject = a_GameObject;

		if (m_isEnabled)
		{
			OnEnable();
		}
		else
		{
			OnDisable();
		}
    }

    void Component::Update(float a_DeltaTime)
    {
        a_DeltaTime;
    }

    void Component::FixedUpdate(float a_TimeStep)
    {
        a_TimeStep;
    }

	void Component::Enable()
	{
		m_isEnabled = true;
        OnEnable();
	}

	void Component::Disable()
	{
		m_isEnabled = false;
        OnDisable();
	}

    TransformComponent* Component::GetTransform() const
    {
		return m_GameObject->GetTransform();
    }

    void Component::OnEnable()
	{
	}

	void Component::OnDisable()
	{
	}

}

RTTR_REGISTRATION
{
	rttr::registration::class_<Muse::Component>("Component")
		.constructor<>()
		(
			rttr::policy::ctor::as_raw_ptr
		)
		.property("Enabled", &Muse::Component::m_isEnabled);
}