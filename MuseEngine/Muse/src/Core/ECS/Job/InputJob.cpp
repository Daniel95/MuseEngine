#include "MusePCH.h"
#include "InputJob.h"
#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Event/Event.h"
#include "Core/Utilities/Defines.h"
#include "Core/Input/Input.h"

namespace Muse
{
    InputJob::InputJob()
    {
        m_Input = std::make_unique<Input>();

        Application::Get().GetWindow()->MouseScrolledEvent.Subscribe(SUB_FN(InputJob::OnMouseScrolledEvent, std::placeholders::_1, std::placeholders::_2));
    }

    InputJob::~InputJob()
    {
        Application::Get().GetWindow()->MouseScrolledEvent.Unsubscribe(this);
    }

    void InputJob::OnUpdate()
    {
        m_Input->UpdateMouseScrollDelta(0);
    }

    void InputJob::OnMouseScrolledEvent(float a_xOffset, float a_yOffset)
    {
        m_Input->UpdateMouseScrollDelta(a_yOffset);
    }
}

