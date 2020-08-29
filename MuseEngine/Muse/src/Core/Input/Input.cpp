#include "MusePCH.h"
#include "Input.h"

#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Utilities/Defines.h"
#include "Core/Event/Event.h"

namespace Muse
{
    float Input::s_MouseScrollDelta = 0;
    bool Input::s_BlockInput = false;

    void Input::UpdateMouseScrollDelta(float a_MouseScrollDelta)
    {
        if (!s_BlockInput)
        {
            s_MouseScrollDelta = a_MouseScrollDelta;
        }
        else
        {
            s_MouseScrollDelta = 0;
        }
    }
}