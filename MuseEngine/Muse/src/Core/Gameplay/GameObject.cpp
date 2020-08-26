#include "MusePCH.h"

#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/TransformComponentOld.h"
#include "Core/Scene/SceneOld.h"
#include "Core/Instrumentor.h"

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

namespace Muse 
{
    GameObject::~GameObject()
    {
        MUSE_PROFILE_FUNCTION();

        for (auto component : m_Components)
        {
            component.reset();
        }
    }

    void GameObject::Init(SceneOld& a_Scene)
    {
        MUSE_PROFILE_FUNCTION();

        m_Scene = &a_Scene;

        m_TransformComponent = GetComponent<TransformComponentOld>();

        if (m_TransformComponent == nullptr)
        {
            m_TransformComponent = AddComponent<TransformComponentOld>();
        }
    }

    void GameObject::Update(float a_DeltaTime)
    {
        MUSE_PROFILE_FUNCTION();

        for (auto component : m_Components)
        {
			if (component->IsEnabled())
			{
				component->Update(a_DeltaTime);
			}
        }
    }

    void GameObject::FixedUpdate(float a_TimeStep)
    {
        MUSE_PROFILE_FUNCTION();

        for (auto component : m_Components)
        {
            if (component->IsEnabled())
            {
                component->FixedUpdate();
            }
        }
    }

    void GameObject::Destroy()
    {
        MUSE_PROFILE_FUNCTION();

        if (!m_Destroyed)
        {
            m_Scene->RemoveGameObject(shared_from_this());
            m_Destroyed = true;
        }
    }

    void GameObject::Enable()
    {
        MUSE_PROFILE_FUNCTION();

        for (auto component : m_Components)
        {
            component->Enable();
        }
    }

    void GameObject::Disable()
    {
        MUSE_PROFILE_FUNCTION();

        for (auto component : m_Components)
        {
            component->Disable();
        }
    }
}

RTTR_REGISTRATION
{
    rttr::registration::class_<Muse::GameObject>("GameObject")
        .constructor<>()
        (
            rttr::policy::ctor::as_raw_ptr
        ).property("Components", &Muse::GameObject::m_Components);
}