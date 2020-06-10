#include "MusePCH.h"

#include "Core/Gameplay/Component/Component.h"
#include "Core/Gameplay/GameObject.h"
#include "TransformComponentOld.h"
#include "Core/Instrumentor.h"
#include <utility>

#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>

namespace Muse 
{
    void Component::Init(std::shared_ptr<GameObject> a_GameObject)
    {
		MUSE_PROFILE_FUNCTION();

        m_GameObject = a_GameObject;

		m_Initialized = true;

		if (m_isEnabled)
		{
			OnEnable();
		}
		else
		{
			OnDisable();
		}

		OnInit();
    }

	void Component::Enable()
	{
		MUSE_PROFILE_FUNCTION();

		m_isEnabled = true;
		if(!m_isEnabled)
		{
			OnEnable();
		}
	}

	void Component::Disable()
	{
		MUSE_PROFILE_FUNCTION();
 
		m_isEnabled = false;
		if (m_isEnabled)
		{
			OnDisable();
		}
	}

    std::shared_ptr<TransformComponentOld> Component::GetTransform() const
    {
		return m_GameObject->GetTransform();
    }
}

RTTR_REGISTRATION
{
	rttr::registration::class_<Muse::Component>("Component")
		.constructor<>()
		(
			rttr::policy::ctor::as_raw_ptr
		);
		//.property("Enabled", &Muse::Component::m_isEnabled);
}