#pragma once

#include "Core/ECS/Job/Job.h"

namespace Muse
{
    class Input;

    class InputJob : public Job
    {
    public:
        InputJob();
        ~InputJob();

        void OnUpdate() override;

    private:
        void OnMouseScrolledEvent(float a_xOffset, float a_yOffset);

        std::unique_ptr<Input> m_Input;

    };
}
