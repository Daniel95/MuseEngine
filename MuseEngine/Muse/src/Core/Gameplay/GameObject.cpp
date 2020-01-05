#include "MusePCH.h"

#include "Core/Gameplay/Component/TransformComponent.h"
#include "Core/Scene/Scene.h"
#include "Core/Instrumentor.h"

namespace Muse 
{
    GameObject::~GameObject()
    {
        MUSE_PROFILE_FUNCTION();

        for (Component* component : m_Components)
        {
            delete component;
        }
    }

    void GameObject::Init(Scene& a_Scene)
    {
        MUSE_PROFILE_FUNCTION();

        m_Scene = &a_Scene;

        if (!HasComponent<TransformComponent>())
        {
            AddComponent<TransformComponent>();
        }
    }

    void GameObject::Update(float a_DeltaTime)
    {
        MUSE_PROFILE_FUNCTION();

        for (Component* component : m_Components)
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

        for (Component* component : m_Components)
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
            m_Scene->RemoveGameObject(*this);
            m_Destroyed = true;
        }
    }

    void GameObject::Enable()
    {
        MUSE_PROFILE_FUNCTION();

        for (Component* component : m_Components)
        {
            component->Enable();
        }
    }

    void GameObject::Disable()
    {
        MUSE_PROFILE_FUNCTION();

        for (Component* component : m_Components)
        {
            component->Disable();
        }
    }

    Scene* GameObject::GetScene() const
    {
        return m_Scene;
    }

    TransformComponent* GameObject::GetTransform() const
    {
        return GetComponent<TransformComponent>();
    }

    void GameObject::SetComponents(const std::vector<Component*>& a_Components)
    {
        m_Components = a_Components;
    }

    const std::vector<Component*>& GameObject::GetComponents()
    {
        return m_Components;
    }

    RTTR_REGISTRATION
    {
        rttr::registration::class_<GameObject>("GameObject")
            .constructor<>()
            (
                rttr::policy::ctor::as_raw_ptr
            )
            .property("Components", &GameObject::GetComponents, &GameObject::SetComponents);
    }
}
