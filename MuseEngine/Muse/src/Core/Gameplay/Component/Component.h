#pragma once

#include <rttr/registration>
#include <rttr/registration_friend>

namespace Muse
{
    class GameObject;
    class BoxCollider2D;

    class Component
    {
        RTTR_ENABLE();

    public:
        Component();
        virtual ~Component();

        virtual void Init(GameObject* a_GameObject);
        virtual void Update(float a_DeltaTime);
        virtual void FixedUpdate(float a_TimeStep);

		void Enable();
        void Disable();

        GameObject* GetGameObject() const { return m_GameObject; }
		const bool& IsEnabled() { return m_isEnabled; };

		RTTR_REGISTRATION_FRIEND;

	protected:
		virtual void OnEnable();
		virtual void OnDisable();

    private:
        GameObject* m_GameObject;
		bool m_isEnabled = true;
    };
}
