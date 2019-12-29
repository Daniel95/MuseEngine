#pragma once

#include <rttr/registration>
#include <rttr/registration_friend>

namespace Muse
{
    class GameObject;
    class BoxCollider2D;
    class TransformComponent;

    class Component
    {
        RTTR_ENABLE();

    public:
        Component() = default;
        virtual ~Component() = default;

        virtual void Init(GameObject* a_GameObject);
        virtual void Update(float a_DeltaTime);
        virtual void FixedUpdate(float a_TimeStep);

		void Enable();
        void Disable();

        GameObject* GetGameObject() const { return m_GameObject; }
        TransformComponent* GetTransform() const;
		bool IsEnabled() const { return m_isEnabled; };

		RTTR_REGISTRATION_FRIEND;

	protected:
		virtual void OnEnable();
		virtual void OnDisable();

    private:
        GameObject* m_GameObject = nullptr;
		bool m_isEnabled = true;

    };
}
