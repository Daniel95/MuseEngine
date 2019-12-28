#pragma once
#include "Component.h"

namespace Muse
{
    class CameraComponent : public Component
    {
    public:
        CameraComponent() = default;
        virtual ~CameraComponent() = default;

        void MakeEditorCamera() { m_IsEditorCamera = true; }
        bool IsEditorCamera() const { return m_IsEditorCamera; }

    private:
        bool m_IsEditorCamera = false;

    };
}
