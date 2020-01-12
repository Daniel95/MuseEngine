#pragma once

#include "Core/Input/Input.h"

namespace Muse
{
    class WindowsInput : public Input
    {
    protected:
        virtual bool GetKeyDownImpl(int a_Keycode) const override;
        virtual bool GetMouseButtonDownImpl(int a_Button) const override;
        virtual glm::vec2 GetMousePositionImpl() const override;
        virtual float GetMouseXImpl() const override;
        virtual float GetMouseYImpl() const override;
    };
}

